#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0
"""
Generate deterministic input/golden fixtures for the snitchCluster `matmul` test.

The kernel under test is `MatMul_unrolled_2x2_parallel_s8_rv32im()` in
`src_cluster/test_cluster.c`. It computes, in exact integer arithmetic,

    C[i][j] = sum_k A[i * MAT_N + k] * B[k * MAT_P + j]

with A, B of type `int8_t` and C of type `int32_t`. The 2x2 unrolling and the
distribution of rows across the compute cores do not change the result, so the
reference model is a plain integer matrix product.
"""

from __future__ import annotations

import argparse
from pathlib import Path

import numpy as np

# clang-format settings this repository uses (see .clang-format). They are
# reproduced here so that the emitted headers need no post-processing.
CLANG_FORMAT_COLUMN_LIMIT = 100
CLANG_FORMAT_INDENT_WIDTH = 4

# int8 operands are drawn from `np.random.randint(-128, 127)`, i.e. the value
# 127 never occurs.
INT8_LOW = -128
INT8_HIGH = 127

# Per-shape description of the committed fixtures.
#
# `blocks`   number of independent matmul problems stored back-to-back.
# `rng_skip` number of samples consumed from the seed-0 stream before the
#            operands of this shape. The 32x48x64 vectors predate this script
#            and sit at a fixed offset in the stream; skipping that many
#            samples is what makes the regenerated files byte-identical to the
#            committed ones.
# `eof_newline` whether the committed file ends with a newline.
# Provenance emitted into each generated header, so the origin of the data
# travels with the data rather than living only in review comments.
DEEPLOY_PROVENANCE = ("//\n"
                      "// GENERATED DATA -- DO NOT EDIT BY HAND. Rebuild with generate_vectors.py\n"
                      "// (`cmake --build <build> --target regenerate-test-vectors`).\n"
                      "//\n"
                      "// Source   : Deeploy test DeeployTest/Tests/Kernels/Integer/MatMul/Regular,\n"
                      "//            reproduced byte-for-byte. Verified against Deeploy 4ee0555a.\n"
                      "// Note     : the Deeploy export has batch 2; only batch 0 is used and checked.\n")

SHAPES = {
    "32x48x64": {
        "M": 32,
        "N": 48,
        "P": 64,
        "blocks": 2,
        "rng_skip": 169714,
        "eof_newline": True,
        "provenance": DEEPLOY_PROVENANCE,
    },
    "16x16x16": {
        "M": 16,
        "N": 16,
        "P": 16,
        "blocks": 1,
        "rng_skip": 0,
        "eof_newline": False,
        "provenance": "",
    },
}

# Set random seeds
np.random.seed(0)


def gen_operands(shape: dict) -> tuple[np.ndarray, np.ndarray]:
    """Draw the A and B operands of one shape from the seed-0 stream."""
    np.random.seed(0)
    if shape["rng_skip"]:
        np.random.randint(INT8_LOW, INT8_HIGH, size = shape["rng_skip"], dtype = np.int64)
    a = np.random.randint(INT8_LOW, INT8_HIGH, size = (shape["blocks"], shape["M"], shape["N"]), dtype = np.int64)
    b = np.random.randint(INT8_LOW, INT8_HIGH, size = (shape["blocks"], shape["N"], shape["P"]), dtype = np.int64)
    return a, b


def matmul(a: np.ndarray, b: np.ndarray) -> np.ndarray:
    """Integer reference model of MatMul_unrolled_2x2_parallel_s8_rv32im().

    a: [blocks, M, N] int8-valued
    b: [blocks, N, P] int8-valued
    returns: [blocks, M, P] int32
    """
    out = np.matmul(a.astype(np.int64), b.astype(np.int64))
    assert np.all(out >= np.iinfo(np.int32).min) and np.all(out <= np.iinfo(np.int32).max)
    return out.astype(np.int32)


def _column_layout(items: list[str], closer: str = "};") -> tuple[int, list[int]]:
    """Reproduce clang-format's `CommaSeparatedList` column layout.

    Returns the number of columns and the width of each column. clang-format
    lays a long braced initializer out as a grid: every item is padded to the
    widest item of its column, and the widest layout that still fits into the
    column limit wins.
    """
    count = len(items)
    item_lengths = []
    end_of_line_lengths = []
    for index, text in enumerate(items):
        last = index == count - 1
        # Non-final items carry a trailing comma, the final one carries "};".
        item_lengths.append(len(text) if last else len(text) + 1)
        end_of_line_lengths.append(len(text) + len(closer) if last else len(text) + 1)

    formats = []
    for columns in range(1, CLANG_FORMAT_COLUMN_LIMIT // 3 + 1):
        column_sizes = [0] * columns
        column_min_sizes = [max(end_of_line_lengths)] * columns
        line_count = 1
        has_full_row = False
        column = 0
        for index in range(count):
            if column == columns:
                line_count += 1
                column = 0
            if column == columns - 1:
                has_full_row = True
            length = end_of_line_lengths[index] if column == columns - 1 else item_lengths[index]
            column_sizes[column] = max(column_sizes[column], length)
            column_min_sizes[column] = min(column_min_sizes[column], length)
            column += 1
        if not has_full_row:
            break
        # Skip layouts whose columns would need excessive padding.
        if any(column_sizes[i] - column_min_sizes[i] > 10 for i in range(columns - 1)):
            continue
        total_width = columns - 1 + sum(column_sizes)
        if total_width > CLANG_FORMAT_COLUMN_LIMIT and columns > 1:
            continue
        formats.append((columns, line_count, total_width, column_sizes))

    remaining = CLANG_FORMAT_COLUMN_LIMIT - CLANG_FORMAT_INDENT_WIDTH
    best = None
    for candidate in reversed(formats):
        if candidate[2] <= remaining or candidate[0] == 1:
            if best is not None and candidate[1] > best[1]:
                break
            best = candidate
    return best[0], best[3]


def format_array(arr: np.ndarray, closer: str = "};") -> str:
    """Render a flat array the way clang-format would render it."""
    # The committed fixtures spell the integer initializers as float literals.
    items = ["{}.0".format(int(value)) for value in arr.reshape(-1)]
    columns, column_sizes = _column_layout(items, closer)
    count = len(items)
    lines = []
    for start in range(0, count, columns):
        chunk = items[start:start + columns]
        cells = []
        for offset, text in enumerate(chunk):
            index = start + offset
            cell = text + (closer if index == count - 1 else ",")
            if offset == len(chunk) - 1 or index == count - 1:
                cells.append(cell)
            else:
                cells.append(cell.ljust(column_sizes[offset]) + " ")
        lines.append(" " * CLANG_FORMAT_INDENT_WIDTH + "".join(cells))
    return "\n".join(lines)


def build_inputs_body(shape: dict, a: np.ndarray, b: np.ndarray) -> str:
    return ("// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna\n"
            "// SPDX-License-Identifier: Apache-2.0\n"
            f"{shape['provenance']}"
            "\n"
            "#include <stdint.h>\n"
            "\n"
            f"#define MAT_M {shape['M']}\n"
            f"#define MAT_N {shape['N']}\n"
            f"#define MAT_P {shape['P']}\n"
            "#define MAT_OPS (2ULL * MAT_M * MAT_N * MAT_P) // 2 ops per MAC\n"
            "\n"
            'int8_t __attribute__((__section__(".cdata"))) testInputVector0[] = {\n'
            f"{format_array(a)}\n"
            'int8_t __attribute__((__section__(".cdata"))) testInputVector1[] = {\n'
            f"{format_array(b)}")


def build_outputs_body(c: np.ndarray, provenance: str = "") -> str:
    return ("// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna\n"
            "// SPDX-License-Identifier: Apache-2.0\n"
            f"{provenance}"
            "\n"
            "#include <stdint.h>\n"
            "\n"
            'int32_t testOutputVector0[] __attribute__((__section__(".cdata"))) = {\n'
            f"{format_array(c)}")


def main() -> None:
    include_dir = Path(__file__).parent / "include"
    parser = argparse.ArgumentParser(description = "Generate matmul test fixtures")
    for shape_name in SHAPES:
        default_inputs = include_dir / f"testinputs_{shape_name}.h"
        parser.add_argument(
            f"--output-inputs-{shape_name}",
            type = Path,
            default = default_inputs,
            help = f"Path to write the {shape_name} inputs header (default: {default_inputs})",
        )
        default_outputs = include_dir / f"testoutputs_{shape_name}.h"
        parser.add_argument(
            f"--output-outputs-{shape_name}",
            type = Path,
            default = default_outputs,
            help = f"Path to write the {shape_name} golden header (default: {default_outputs})",
        )
    args = parser.parse_args()

    for shape_name, shape in SHAPES.items():
        a, b = gen_operands(shape)
        c = matmul(a, b)
        print(f"[{shape_name}] A {a.shape} B {b.shape} C {c.shape}")

        suffix = "\n" if shape["eof_newline"] else ""
        inputs_path = getattr(args, f"output_inputs_{shape_name}".replace("-", "_"))
        inputs_path.write_text(build_inputs_body(shape, a, b) + suffix)
        outputs_path = getattr(args, f"output_outputs_{shape_name}".replace("-", "_"))
        outputs_path.write_text(build_outputs_body(c, shape["provenance"]) + suffix)
        print(f"[{shape_name}] wrote {inputs_path} and {outputs_path}")


if __name__ == "__main__":
    main()
