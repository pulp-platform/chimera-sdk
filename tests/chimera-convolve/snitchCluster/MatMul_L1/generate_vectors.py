#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0
"""
Generate deterministic input/golden fixtures for MatMul_L1.

The kernel under test (``MatMul_unrolled_2x2_parallel_s8_rv32im`` in
``src_cluster/test_cluster.c``) computes a plain integer matrix product::

    pDstC[i * P + j] = sum_k pSrcA[i * N + k] * pSrcB[k * P + j]

with ``pSrcA`` (M x N) and ``pSrcB`` (N x P) of type ``int8_t`` and ``pDstC``
(M x P) of type ``int32_t``. The 2x2 unrolling and the round-robin distribution
of row pairs over the compute cores only reorder the additions, they never
change a result: both operands are promoted to ``int`` before the multiply and
every accumulator stays far inside ``int32_t`` (``N * 128 * 128 <= 786432``).
"""

from __future__ import annotations

import argparse
from pathlib import Path

import numpy as np

# int8 operands are drawn from [INT8_MIN, INT8_MAX), i.e. 127 never occurs
INT8_LOW = -128
INT8_HIGH = 127

# Per-shape description of the committed fixtures.
#
# ``blocks``      number of independent matmul problems stored back to back.
# ``rng_skip``    samples consumed from the seed-0 stream before this shape's
#                 operands. The 32x48x64 vectors predate this script and sit at
#                 a fixed offset inside that stream; skipping this many samples
#                 first is what reproduces the committed file byte for byte.
# ``eof_newline`` whether the committed header ends with a trailing newline
#                 (the two shapes disagree, so it is recorded per shape).
# Provenance emitted into each generated header, so the origin of the data
# travels with the data rather than living only in review comments.
DEEPLOY_PROVENANCE = ("//\n"
                      "// GENERATED DATA -- DO NOT EDIT BY HAND. Rebuild with generate_vectors.py\n"
                      "// (`cmake --build <build> --target regenerate-test-vectors`).\n"
                      "//\n"
                      "// Source   : Deeploy test DeeployTest/Tests/Kernels/Integer/MatMul/Regular,\n"
                      "//            reproduced byte-for-byte. Verified against Deeploy 4ee0555a.\n"
                      "// Note     : the Deeploy export has batch 2; only batch 0 is used and checked.\n")

SHAPES = (
    {
        "M": 16,
        "N": 16,
        "P": 16,
        "blocks": 1,
        "rng_skip": 0,
        "eof_newline": False,
        "provenance": ""
    },
    {
        "M": 32,
        "N": 48,
        "P": 64,
        "blocks": 2,
        "rng_skip": 169714,
        "eof_newline": True,
        "provenance": DEEPLOY_PROVENANCE
    },
)

# Emitted C formatting (mirrors the repository .clang-format)
COLUMN_LIMIT = 100
INDENT_WIDTH = 4

# Set random seeds
SEED = 0
np.random.seed(SEED)


def gen_operands(shape: dict) -> tuple[np.ndarray, np.ndarray]:
    """Draw the A and B operands of one shape from the seed-0 stream."""
    # Reseeded per shape so every header depends only on its own entry above
    np.random.seed(SEED)
    if shape["rng_skip"]:
        np.random.randint(INT8_LOW, INT8_HIGH, size = shape["rng_skip"], dtype = np.int64)
    src_a = np.random.randint(INT8_LOW, INT8_HIGH, size = (shape["blocks"], shape["M"], shape["N"]), dtype = np.int64)
    src_b = np.random.randint(INT8_LOW, INT8_HIGH, size = (shape["blocks"], shape["N"], shape["P"]), dtype = np.int64)
    return src_a.astype(np.int8), src_b.astype(np.int8)


def matmul(src_a: np.ndarray, src_b: np.ndarray) -> np.ndarray:
    """
    Reference model of ``MatMul_unrolled_2x2_parallel_s8_rv32im``.

    src_a: [blocks, M, N] int8
    src_b: [blocks, N, P] int8
    returns: [blocks, M, P] int32
    """
    out = np.matmul(src_a.astype(np.int64), src_b.astype(np.int64))
    info = np.iinfo(np.int32)
    assert np.all(out >= info.min) and np.all(out <= info.max), "int32 accumulator overflow"
    return out.astype(np.int32)


def _column_layout(item_lengths: list[int]) -> tuple[int, list[int]]:
    """
    Reproduce clang-format's column layout for a braced initializer list.

    Mirrors ``CommaSeparatedList::precomputeFormattingInfos`` and
    ``getColumnFormat``: for every candidate number of columns, a column is as
    wide as its widest item, columns are separated by one space, and the layout
    with the most columns that still fits the remaining line width wins.
    """
    formats = []
    for columns in range(1, COLUMN_LIMIT // 3 + 1):
        sizes = [0] * columns
        min_sizes = [max(item_lengths)] * columns
        line_count = 1
        has_full_row = False
        column = 0
        for length in item_lengths:
            if column == columns:
                line_count += 1
                column = 0
            if column == columns - 1:
                has_full_row = True
            sizes[column] = max(sizes[column], length)
            min_sizes[column] = min(min_sizes[column], length)
            column += 1
        if not has_full_row:
            break
        # Reject layouts whose columns would need excessive padding
        if any(sizes[i] - min_sizes[i] > 10 for i in range(columns - 1)):
            continue
        total_width = columns - 1 + sum(sizes)
        if total_width > COLUMN_LIMIT and columns > 1:
            continue
        formats.append((columns, sizes, total_width, line_count))

    best = None
    for candidate in reversed(formats):
        if candidate[2] <= COLUMN_LIMIT - INDENT_WIDTH or candidate[0] == 1:
            if best is not None and candidate[3] > best[3]:
                break
            best = candidate
    assert best is not None, "no viable column layout"
    return best[0], best[1]


def format_array(arr: np.ndarray) -> str:
    """Render a flat array the way clang-format lays out the committed headers."""
    items = [f"{int(v)}.0" for v in arr.flatten().tolist()]
    # The closing brace directly follows the last value, so it carries no comma
    items = [item + "," for item in items[:-1]] + items[-1:]
    columns, sizes = _column_layout([len(item) for item in items])
    lines = []
    for start in range(0, len(items), columns):
        chunk = items[start:start + columns]
        row = "".join(item.ljust(sizes[i]) + " " for i, item in enumerate(chunk))
        lines.append((" " * INDENT_WIDTH + row).rstrip())
    return "\n".join(lines)


def build_testinputs_body(shape: dict, src_a: np.ndarray, src_b: np.ndarray) -> str:
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
            f"{format_array(src_a)}}};\n"
            'int8_t __attribute__((__section__(".cdata"))) testInputVector1[] = {\n'
            f"{format_array(src_b)}}};" + ("\n" if shape["eof_newline"] else ""))


def build_testoutputs_body(shape: dict, golden: np.ndarray) -> str:
    return ("// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna\n"
            "// SPDX-License-Identifier: Apache-2.0\n"
            f"{shape['provenance']}"
            "\n"
            "#include <stdint.h>\n"
            "\n"
            'int32_t testOutputVector0[] __attribute__((__section__(".cdata"))) = {\n'
            f"{format_array(golden)}}};" + ("\n" if shape["eof_newline"] else ""))


def shape_name(shape: dict) -> str:
    return f"{shape['M']}x{shape['N']}x{shape['P']}"


def main() -> None:
    parser = argparse.ArgumentParser(description = "Generate MatMul_L1 test vector headers")
    include_dir = Path(__file__).parent / "include"
    for shape in SHAPES:
        name = shape_name(shape)
        default_inputs = include_dir / f"testinputs_{name}.h"
        parser.add_argument(
            f"--output-inputs-{name}",
            type = Path,
            default = default_inputs,
            help = f"Path to write the generated {name} input header (default: {default_inputs})",
        )
        default_outputs = include_dir / f"testoutputs_{name}.h"
        parser.add_argument(
            f"--output-outputs-{name}",
            type = Path,
            default = default_outputs,
            help = f"Path to write the generated {name} golden header (default: {default_outputs})",
        )
    args = parser.parse_args()

    for shape in SHAPES:
        name = shape_name(shape)
        src_a, src_b = gen_operands(shape)
        print(f"[{name}] Input 0 Shape:", src_a.shape)
        print(f"[{name}] Input 1 Shape:", src_b.shape)

        golden = matmul(src_a, src_b)
        print(f"[{name}] Golden Shape:", golden.shape)

        path_inputs = getattr(args, f"output_inputs_{name}")
        path_outputs = getattr(args, f"output_outputs_{name}")
        path_inputs.parent.mkdir(parents = True, exist_ok = True)
        path_outputs.parent.mkdir(parents = True, exist_ok = True)
        path_inputs.write_text(build_testinputs_body(shape, src_a, src_b))
        path_outputs.write_text(build_testoutputs_body(shape, golden))
        print(f"[{name}] Wrote {path_inputs} with {src_a.size} + {src_b.size} input entries")
        print(f"[{name}] Wrote {path_outputs} with {golden.size} golden entries")


if __name__ == "__main__":
    main()
