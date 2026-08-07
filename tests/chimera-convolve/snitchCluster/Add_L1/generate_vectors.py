#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0
"""
Generate deterministic input/golden fixtures for Add_L1.

The kernel under test (``SnitchAdd`` in ``src_cluster/test_cluster.c``) performs a
plain elementwise addition over ``ADD_M * ADD_N`` elements::

    pOut[i] = pIn1[i] + pIn2[i];

with ``pIn1``/``pIn2`` of type ``int8_t *`` and ``pOut`` of type ``int32_t *``.
Both operands are promoted to ``int`` before the addition, so the sum never
wraps and never saturates: the exact reference model is a widening int32 add.
"""

from __future__ import annotations

import argparse
from pathlib import Path

import numpy as np

# Tensor dimensions (mirrors the ADD_M / ADD_N defines emitted into the header)
ADD_M = 128
ADD_N = 128

# int8 operands are drawn from [INT8_MIN, INT8_MAX), matching the committed data
INT8_LOW = -128
INT8_HIGH = 127

# Emitted C formatting (matches clang-format's column alignment of the committed file)
VALUES_PER_LINE = 12
CELL_WIDTH = 8

# Set random seeds
np.random.seed(0)


def gen_input() -> np.ndarray:
    return np.random.randint(INT8_LOW, INT8_HIGH, size = (ADD_M, ADD_N), dtype = np.int8)


def add(in0: np.ndarray, in1: np.ndarray) -> np.ndarray:
    """
    Reference model of ``SnitchAdd``.

    in0: [ADD_M, ADD_N] int8
    in1: [ADD_M, ADD_N] int8
    returns: [ADD_M, ADD_N] int32
    """
    return in0.astype(np.int32) + in1.astype(np.int32)


def format_array(arr: np.ndarray) -> str:
    vals = arr.flatten().tolist()
    lines = []
    for start in range(0, len(vals), VALUES_PER_LINE):
        chunk = vals[start:start + VALUES_PER_LINE]
        row = "".join(f"{int(v)}.0,".ljust(CELL_WIDTH) for v in chunk)
        lines.append(("    " + row).rstrip())
    # The closing brace directly follows the last value, so drop its trailing comma
    return "\n".join(lines)[:-1]


def build_testdata_body(in0: np.ndarray, in1: np.ndarray, out: np.ndarray) -> str:
    return ("// SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna\n"
            "// SPDX-License-Identifier: Apache-2.0\n"
            "\n"
            "#include <stdint.h>\n"
            "\n"
            f"#define ADD_M {ADD_M}\n"
            f"#define ADD_N {ADD_N}\n"
            "#define ADD_OPS (ADD_M * ADD_N)\n"
            "\n"
            'int8_t __attribute__((__section__(".cdata"))) testInputVector0[] = {\n'
            f"{format_array(in0)}}};\n"
            'int8_t __attribute__((__section__(".cdata"))) testInputVector1[] = {\n'
            f"{format_array(in1)}}};\n"
            "\n"
            "int32_t testOutputVector0[] = {\n"
            f"{format_array(out)}}};")


def main() -> None:
    parser = argparse.ArgumentParser(description = "Generate testdata_128x128.h fixtures")
    default_output_testdata = Path(__file__).parent / "include" / f"testdata_{ADD_M}x{ADD_N}.h"
    parser.add_argument(
        "--output-testdata",
        type = Path,
        default = default_output_testdata,
        help = f"Path to write the generated C data header (default: {default_output_testdata})",
    )
    args = parser.parse_args()

    in0 = gen_input()
    print("Input 0 Shape:", in0.shape)
    in1 = gen_input()
    print("Input 1 Shape:", in1.shape)

    out = add(in0, in1)
    print("Golden Shape:", out.shape)

    args.output_testdata.parent.mkdir(parents = True, exist_ok = True)
    args.output_testdata.write_text(build_testdata_body(in0, in1, out))
    print(f"Wrote {args.output_testdata} with {in0.size} + {in1.size} inputs and {out.size} golden entries")

    np.set_printoptions(linewidth = 200, threshold = 10000, formatter = {"int": "{:5}".format})
    print("\nInput 0:")
    print(in0)
    print("\nInput 1:")
    print(in1)
    print("\nGolden:")
    print(out)


if __name__ == "__main__":
    main()
