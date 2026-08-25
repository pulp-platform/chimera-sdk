#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0
"""
Generate deterministic input/scale/golden fixtures for LayerScale_L1.

The kernel under test is SnitchRequantShift_s8_s8_NHWC (see
src_cluster/test_cluster.c), invoked as

    SnitchRequantShift_s8_s8_NHWC(input_tensor, 6144, mul_tensor, add_tensor,
                                  output, 16, 1);

i.e. size = LAYERSCALE_M * LAYERSCALE_N = 6144, log2D = 16 and channels = 1.
Because channels == 1, only mul_tensor[0] and add_tensor[0] are ever read; the
remaining LAYERSCALE_N - 1 entries are padding kept for layout compatibility.

The exact integer reference model is therefore, for every element i:

    intermediate = (int32_t)input_tensor[i] * mul_tensor[0] + add_tensor[0]
    intermediate = (intermediate + (1 << (log2D - 1))) >> log2D   # arithmetic
    expected_output[i] = CLAMP(intermediate, -128, 127)
"""

from __future__ import annotations

import argparse
from pathlib import Path
from typing import List, Sequence

import numpy as np

# Tensor dimensions (mirror the LAYERSCALE_* macros in testdata_16x384.h)
LAYERSCALE_M = 16
LAYERSCALE_N = 384
LAYERSCALE_SIZE = LAYERSCALE_M * LAYERSCALE_N

# Requantization parameters (mirror the call site in src_cluster/test_cluster.c)
REQ_MUL = 269854
REQ_ADD = 0
REQ_LOG2D = 16
REQ_CHANNELS = 1

# Standard deviation of the synthetic activations, matching the committed data
INPUT_SIGMA = 31

# clang-format settings the emitted header must satisfy (see .clang-format)
CLANG_FORMAT_INDENT = 4
CLANG_FORMAT_COLUMN_LIMIT = 100

# Set random seeds
np.random.seed(0)


def gen_input() -> np.ndarray:
    """Draw a deterministic, roughly Gaussian int8 activation tensor."""
    raw = np.random.randn(LAYERSCALE_SIZE) * INPUT_SIGMA
    return np.clip(np.rint(raw), -128, 127).astype(np.int8)


def gen_mul() -> np.ndarray:
    return np.full(LAYERSCALE_N, REQ_MUL, dtype = np.int32)


def gen_add() -> np.ndarray:
    return np.full(LAYERSCALE_N, REQ_ADD, dtype = np.int32)


def requant_shift(data_in: np.ndarray, mul: np.ndarray, add: np.ndarray, log2d: int, channels: int) -> np.ndarray:
    """
    Bit-exact model of SnitchRequantShift_s8_s8_NHWC.

    Args:
        data_in (np.ndarray): Flat int8 input tensor of length size.
        mul (np.ndarray): Per-channel int32 multipliers.
        add (np.ndarray): Per-channel int32 offsets.
        log2d (int): Right shift amount applied after rounding.
        channels (int): Number of channels the input is tiled over.

    Returns:
        np.ndarray: Flat int8 output tensor of the same length as data_in.

    Example:
        >>> requant_shift(np.array([1], dtype = np.int8), np.array([269854]),
        ...               np.array([0]), 16, 1)
        array([4], dtype=int8)
    """
    # Cast up to int64 so the multiply can never overflow before the shift
    values = np.asarray(data_in, dtype = np.int64)
    per_channel = np.tile(np.arange(channels), values.size // channels)
    intermediate = values * np.asarray(mul, dtype = np.int64)[per_channel]
    intermediate += np.asarray(add, dtype = np.int64)[per_channel]
    # C right-shifts a negative int32 arithmetically; numpy // on int64 floors,
    # which is the same thing for a power-of-two divisor.
    intermediate = (intermediate + (1 << (log2d - 1))) >> log2d
    return np.clip(intermediate, -128, 127).astype(np.int8)


def format_array(arr: Sequence[int]) -> List[str]:
    """
    Lay out a flat array the way clang-format lays out a braced initializer.

    clang-format picks the largest column count whose per-column widths still
    fit inside ColumnLimit, then pads every entry to its column width.
    """
    tokens = [str(int(x)) for x in arr]
    count = len(tokens)

    def widths(columns: int) -> List[int]:
        result = [0] * columns
        for index, token in enumerate(tokens):
            result[index % columns] = max(result[index % columns], len(token))
        return result

    def line_width(columns: int) -> int:
        # indent + entries + ", " between entries + trailing "," on the last one
        return CLANG_FORMAT_INDENT + sum(widths(columns)) + 2 * (columns - 1) + 1

    columns = 1
    for candidate in range(1, count + 1):
        if line_width(candidate) > CLANG_FORMAT_COLUMN_LIMIT:
            break
        columns = candidate

    column_widths = widths(columns)
    lines = []
    for start in range(0, count, columns):
        row = tokens[start:start + columns]
        pieces = []
        for offset, token in enumerate(row):
            piece = token if start + offset == count - 1 else token + ","
            if offset != len(row) - 1:
                piece = piece.ljust(column_widths[offset] + 2)
            pieces.append(piece)
        lines.append(" " * CLANG_FORMAT_INDENT + "".join(pieces))
    return lines


def emit_array(declaration: str, arr: Sequence[int]) -> str:
    return declaration + " = {\n" + "\n".join(format_array(arr)) + "};\n"


def build_testdata_body(mul: np.ndarray, add: np.ndarray, inp: np.ndarray, golden: np.ndarray) -> str:
    header = ("// SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna\n"
              "// SPDX-License-Identifier: Apache-2.0\n"
              "\n"
              "#include <stdint.h>\n"
              "\n"
              f"#define LAYERSCALE_M {LAYERSCALE_M}\n"
              f"#define LAYERSCALE_N {LAYERSCALE_N}\n"
              "#define LAYERSCALE_OPS (3ULL * LAYERSCALE_N * LAYERSCALE_M)\n"
              "\n")
    section = '__attribute__((__section__(".cdata")))'
    body = header
    body += emit_array(f"int32_t {section} mul_tensor[]", mul) + "\n"
    body += emit_array(f"int32_t {section} add_tensor[]", add) + "\n"
    body += emit_array(f"int8_t {section} input_tensor[]", inp)
    body += emit_array("int8_t expected_output[]", golden)
    return body.rstrip("\n")


def main() -> None:
    parser = argparse.ArgumentParser(description = "Generate LayerScale_L1 test vectors")
    default_output_testdata = Path(__file__).parent / "include" / "testdata_16x384.h"
    parser.add_argument(
        "--output-testdata",
        type = Path,
        default = default_output_testdata,
        help = f"Path to write the generated test data header (default: {default_output_testdata})",
    )
    args = parser.parse_args()

    mul = gen_mul()
    add = gen_add()
    inp = gen_input()
    golden = requant_shift(inp, mul, add, REQ_LOG2D, REQ_CHANNELS)

    print(f"mul_tensor      shape {mul.shape}")
    print(f"add_tensor      shape {add.shape}")
    print(f"input_tensor    shape {inp.shape}")
    print(f"expected_output shape {golden.shape}")
    print(f"saturated outputs: {int(np.count_nonzero(np.abs(golden.astype(np.int32)) >= 127))}"
          f" / {golden.size}")

    args.output_testdata.write_text(build_testdata_body(mul, add, inp, golden))
    print(f"Wrote {args.output_testdata}")


if __name__ == "__main__":
    main()
