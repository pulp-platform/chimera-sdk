#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0
"""
Generate deterministic input/weight/bias/golden fixtures for iLayerNorm_L1.

The golden output is produced by a bit-exact Python transliteration of
``SnitchLayernorm_s8_s8`` (see ``src_cluster/test_cluster.c``), including the
integer square root ``_plp_sqrt_q32_fixed``. The kernel is purely integer, so
the model below matches the C implementation exactly.
"""

from __future__ import annotations

import argparse
from pathlib import Path
from textwrap import dedent

import numpy as np

# Tensor dimensions (mirrors the ILAYERNORM_* defines in the generated header)
ILAYERNORM_M = 16
ILAYERNORM_N = 384

# Kernel invocation parameters (mirrors the call in src_cluster/test_cluster.c)
INPUT_OFFSET = 0
LOG2D = 15

# Distribution parameters of the int32 affine parameters
WEIGHT_MEAN = 2400000
WEIGHT_STD = 240000
BIAS_ABS_MAX = 240000

# Emitted C formatting (matches clang-format with the repository column limit)
C_COLUMN_LIMIT = 100
C_INDENT = 4

# Set random seeds
np.random.seed(0)


def gen_input() -> np.ndarray:
    return np.random.randint(-128, 128, size = (ILAYERNORM_M, ILAYERNORM_N), dtype = np.int8)


def gen_weight() -> np.ndarray:
    weight = np.random.normal(WEIGHT_MEAN, WEIGHT_STD, size = ILAYERNORM_N)
    return np.rint(weight).astype(np.int32)


def gen_bias() -> np.ndarray:
    return np.random.randint(-BIAS_ABS_MAX, BIAS_ABS_MAX, size = ILAYERNORM_N, dtype = np.int32)


def c_div(numerator: int, denominator: int) -> int:
    """Integer division with C semantics (truncation towards zero)."""
    quotient = abs(int(numerator)) // abs(int(denominator))
    return -quotient if (numerator < 0) != (denominator < 0) else quotient


def plp_sqrt_q32_fixed(number: int) -> int:
    """Bit-exact transliteration of ``_plp_sqrt_q32_fixed`` (PULP-DSP)."""
    number = int(number)

    if number <= 0:
        return 1  # Avoid division by zero

    root = 0
    bit = 1 << 30  # Second-to-top bit set

    while bit > number:
        bit >>= 2

    while bit != 0:
        if number >= root + bit:
            number -= root + bit
            root = (root >> 1) + bit
        else:
            root >>= 1
        bit >>= 2

    return root


def layernorm_s8_s8(data_in: np.ndarray, weight: np.ndarray, bias: np.ndarray, input_offset: int, last_dim_length: int,
                    log2d: int) -> np.ndarray:
    """Bit-exact transliteration of ``SnitchLayernorm_s8_s8``."""
    flat_in = data_in.flatten().astype(np.int64)
    flat_out = np.zeros(flat_in.size, dtype = np.int8)
    num_rows = flat_in.size // last_dim_length

    for i in range(num_rows):
        row = flat_in[i * last_dim_length:(i + 1) * last_dim_length] + input_offset

        # Calculate mean
        mean = c_div(int(np.sum(row)), last_dim_length)

        # Calculate variance
        diff = row - mean
        variance = c_div(int(np.sum(diff * diff)), last_dim_length) + 1

        std = plp_sqrt_q32_fixed(variance)

        # Normalize and scale
        for j in range(last_dim_length):
            scaled = c_div(int(diff[j]) * int(weight[j]), std) + int(bias[j])
            output = scaled >> log2d

            # Clip to int8_t range
            output = min(127, max(-128, output))

            flat_out[i * last_dim_length + j] = output

    return flat_out.reshape(data_in.shape)


def format_array(arr: np.ndarray) -> str:
    """Format a flat array the way clang-format lays out the committed fixtures."""
    tokens = [str(int(value)) for value in arr.flatten().tolist()]
    width = max(len(token) for token in tokens) + 2
    per_line = max(1, (C_COLUMN_LIMIT - C_INDENT) // width)

    lines = []
    for start in range(0, len(tokens), per_line):
        chunk = tokens[start:start + per_line]
        parts = []
        for offset, token in enumerate(chunk):
            is_last = (start + offset) == (len(tokens) - 1)
            parts.append(token + "};" if is_last else (token + ",").ljust(width))
        lines.append(" " * C_INDENT + "".join(parts).rstrip())
    return "\n".join(lines)


def build_array(c_type: str, name: str, arr: np.ndarray) -> str:
    declaration = f'{c_type} __attribute__((__section__(".cdata"))) {name}[] = {{'
    return declaration + "\n" + format_array(arr) + "\n"


def build_data_h_body(inp: np.ndarray, weight: np.ndarray, bias: np.ndarray, out: np.ndarray) -> str:
    header = dedent(f"""
        // SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna
        // SPDX-License-Identifier: Apache-2.0

        #include <stdint.h>

        #define ILAYERNORM_M {ILAYERNORM_M}
        #define ILAYERNORM_N {ILAYERNORM_N}
        #define ILAYERNORM_OPS (8ULL * ILAYERNORM_N * ILAYERNORM_M)
        """).strip() + "\n\n"

    return (header + build_array("int32_t", "weight_tensor", weight) + "\n" +
            build_array("int32_t", "bias_tensor", bias) + "\n" + build_array("int8_t", "input_tensor", inp) + "\n" +
            build_array("int8_t", "expected_output", out))


def main() -> None:
    parser = argparse.ArgumentParser(description = "Generate iLayerNorm_L1 test data fixtures")
    default_output_data = Path(__file__).parent / "include" / f"testdata_{ILAYERNORM_M}x{ILAYERNORM_N}.h"
    parser.add_argument(
        "--output-data",
        type = Path,
        default = default_output_data,
        help = f"Path to write the generated C data header (default: {default_output_data})",
    )
    args = parser.parse_args()

    inp = gen_input()
    print("Input Shape:", inp.shape)
    weight = gen_weight()
    print("Weight Shape:", weight.shape)
    bias = gen_bias()
    print("Bias Shape:", bias.shape)

    out = layernorm_s8_s8(inp, weight, bias, INPUT_OFFSET, ILAYERNORM_N, LOG2D)
    print("Golden Shape:", out.shape)

    args.output_data.write_text(build_data_h_body(inp, weight, bias, out))
    print(f"Wrote {args.output_data} with input {inp.size}, weight {weight.size}, "
          f"bias {bias.size}, golden {out.size} entries")


if __name__ == "__main__":
    main()
