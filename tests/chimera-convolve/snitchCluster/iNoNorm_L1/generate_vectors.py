#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0
"""
Generate deterministic input/weight/bias/golden fixtures for iNoNorm_L1.

The kernel under test is ``SnitchiNoNorm_s8_s8`` in ``src_cluster/test_cluster.c``.
It processes four packed int8 values per int32 word and, for every element ``i``:

    partialProduct = (int16_t)(data_in[i] * weights[i])       // int8 * int8, no overflow
    outByte        = (uint8_t)(((partialProduct + bias[i]) * mul) >> log2D)
    data_out[i]    = (int8_t)outByte                          // low byte, wrap-around

``mul`` and ``log2D`` are passed as 425 and 16 by ``inonorm_l1_test``. The shift is an
arithmetic right shift on a signed int32 accumulator and the store truncates (it does
*not* saturate).
"""

from __future__ import annotations

import argparse
from pathlib import Path
from typing import List

import numpy as np

# Tensor dimensions (mirrors the INONORM_M / INONORM_N defines in testinputs_128x128.h)
INONORM_M = 128
INONORM_N = 128
SIZE = INONORM_M * INONORM_N

# Requantization parameters (kept in sync with the SnitchiNoNorm_s8_s8 call in test_cluster.c)
MUL = 425
LOG2D = 16

# Distribution parameters. The RNG used for the originally committed fixtures is unknown;
# these standard deviations reproduce the statistics of the committed data (input is a
# clipped normal with sigma ~64, the weights a normal with sigma ~32 and the per-column
# bias a normal with sigma ~1866).
INPUT_SIGMA = 64.0
WEIGHT_SIGMA = 32.0
BIAS_SIGMA = 1866.0

# C emission parameters (must match .clang-format: ColumnLimit 100, 4 space continuation)
COLUMN_LIMIT = 100
INDENT = 4

# Set random seeds
np.random.seed(0)


def gen_int8(sigma: float, size: int) -> np.ndarray:
    """Draw ``size`` int8 samples from a normal distribution clipped to the int8 range."""
    samples = np.random.normal(0.0, sigma, size = size)
    return np.clip(np.round(samples), -128, 127).astype(np.int8)


def gen_input() -> np.ndarray:
    return gen_int8(INPUT_SIGMA, SIZE)


def gen_weights() -> np.ndarray:
    return gen_int8(WEIGHT_SIGMA, SIZE)


def gen_bias() -> np.ndarray:
    """
    The committed bias tensor holds INONORM_N distinct values broadcast over the
    INONORM_M rows, i.e. one bias per output column.
    """
    bias_vector = np.round(np.random.normal(0.0, BIAS_SIGMA, size = INONORM_N)).astype(np.int32)
    return np.tile(bias_vector, INONORM_M).astype(np.int32)


def inonorm(data_in: np.ndarray, weights: np.ndarray, bias: np.ndarray, mul: int, log2D: int) -> np.ndarray:
    """
    Bit-exact reference model of SnitchiNoNorm_s8_s8.

    Args:
        data_in (np.ndarray): int8 activations.
        weights (np.ndarray): int8 weights.
        bias (np.ndarray): int32 bias, one entry per element.
        mul (int): int32 requantization multiplier.
        log2D (int): arithmetic right shift amount.

    Returns:
        np.ndarray: the int8 output, truncated (not saturated) to the low byte.
    """
    # int8 * int8 always fits in int16, matching the kernel's partialProduct type
    partial_product = (data_in.astype(np.int32) * weights.astype(np.int32)).astype(np.int16)
    accumulator = (partial_product.astype(np.int32) + bias.astype(np.int32)) * np.int32(mul)
    shifted = accumulator >> np.int32(log2D)
    # The kernel stores the result through a uint8_t and packs it into an int32 word,
    # so only the low byte survives and it is read back as a signed int8.
    return (shifted & 0xFF).astype(np.uint8).astype(np.int8)


def format_array(values: np.ndarray, as_float: bool) -> str:
    """
    Emit a clang-format compatible initializer body (table layout, ColumnLimit 100).

    The committed fixtures store their int8_t / int32_t data using float literals; set
    ``as_float`` to reproduce that.
    """
    items: List[str] = [
        ("{:.1f}".format(float(v)) if as_float else "{:d}".format(int(v))) + "," for v in values.flatten().tolist()
    ]
    items[-1] = items[-1][:-1]

    n_columns, widths = 1, [len(items[0])]
    for columns in range(1, len(items) + 1):
        rows = [items[i:i + columns] for i in range(0, len(items), columns)]
        candidate = [max(len(row[c]) for row in rows if c < len(row)) for c in range(columns)]
        if INDENT + sum(w + 1 for w in candidate[:-1]) + candidate[-1] > COLUMN_LIMIT:
            break
        n_columns, widths = columns, candidate

    lines = []
    for i in range(0, len(items), n_columns):
        row = items[i:i + n_columns]
        padded = [item.ljust(widths[c]) for c, item in enumerate(row[:-1])] + [row[-1]]
        lines.append((" " * INDENT + " ".join(padded)).rstrip())
    return "\n".join(lines)


def build_inputs_h_body(data_in: np.ndarray) -> str:
    return ("// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna\n"
            "// SPDX-License-Identifier: Apache-2.0\n"
            "\n"
            "#include <stdint.h>\n"
            "\n"
            f"#define INONORM_N {INONORM_N}\n"
            f"#define INONORM_M {INONORM_M}\n"
            "#define INONORM_OPS (5ULL * INONORM_N * INONORM_M)\n"
            "\n"
            "// int8_t testInputVector0[] = {\n"
            "int8_t __attribute__((__section__(\".cdata\"))) testInputVector0[] = {\n"
            f"{format_array(data_in, as_float = True)}}};\n")


def build_outputs_h_body(golden: np.ndarray) -> str:
    return ("// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna\n"
            "// SPDX-License-Identifier: Apache-2.0\n"
            "\n"
            "#include <stdint.h>\n"
            "\n"
            "int8_t testOutputVector0[] = {\n"
            "    // int8_t testOutputVector0[] __attribute__((__section__(\".cdata\"))) = {\n"
            f"{format_array(golden, as_float = True)}}};")


def build_data_c_body(weights: np.ndarray, bias: np.ndarray) -> str:
    return ("// SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna\n"
            "// SPDX-License-Identifier: Apache-2.0\n"
            "\n"
            "#include <stdint.h>\n"
            "\n"
            f"int8_t __attribute__((__section__(\".cdata\"))) weight_tensor[{weights.size}] = {{\n"
            f"    // int8_t weight_tensor[{weights.size}] = {{\n"
            f"{format_array(weights, as_float = False)}}};\n"
            "\n"
            f"// int32_t bias_tensor[{bias.size}] = {{\n"
            f"int32_t __attribute__((__section__(\".cdata\"))) bias_tensor[{bias.size}] = {{\n"
            f"{format_array(bias, as_float = True)}}};\n")


def main() -> None:
    parser = argparse.ArgumentParser(description = "Generate iNoNorm_L1 fixtures")
    default_output_inputs = Path(__file__).parent / "include" / "testinputs_128x128.h"
    parser.add_argument(
        "--output-inputs",
        type = Path,
        default = default_output_inputs,
        help = f"Path to write the generated input header file (default: {default_output_inputs})",
    )
    default_output_outputs = Path(__file__).parent / "include" / "testoutputs_128x128.h"
    parser.add_argument(
        "--output-outputs",
        type = Path,
        default = default_output_outputs,
        help = f"Path to write the generated golden header file (default: {default_output_outputs})",
    )
    default_output_data = Path(__file__).parent / "src_cluster" / "data.c"
    parser.add_argument(
        "--output-data",
        type = Path,
        default = default_output_data,
        help = f"Path to write the generated weight/bias C file (default: {default_output_data})",
    )
    args = parser.parse_args()

    data_in = gen_input()
    weights = gen_weights()
    bias = gen_bias()
    golden = inonorm(data_in, weights, bias, MUL, LOG2D)

    print(f"Input shape:   ({INONORM_M}, {INONORM_N}) -> {data_in.size} int8 elements")
    print(f"Weight shape:  ({INONORM_M}, {INONORM_N}) -> {weights.size} int8 elements")
    print(f"Bias shape:    ({INONORM_M}, {INONORM_N}) -> {bias.size} int32 elements "
          f"({INONORM_N} unique values broadcast over the rows)")
    print(f"Golden shape:  ({INONORM_M}, {INONORM_N}) -> {golden.size} int8 elements")
    print(f"Golden range:  [{golden.min()}, {golden.max()}]")

    args.output_inputs.write_text(build_inputs_h_body(data_in))
    args.output_outputs.write_text(build_outputs_h_body(golden))
    args.output_data.write_text(build_data_c_body(weights, bias))

    print(f"Wrote {args.output_inputs}")
    print(f"Wrote {args.output_outputs}")
    print(f"Wrote {args.output_data}")


if __name__ == "__main__":
    main()
