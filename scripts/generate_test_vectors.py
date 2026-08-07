#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0
"""
Regenerate every generated test vector in this repository.

Run this once after cloning, and again whenever a generator changes::

    python scripts/generate_test_vectors.py                # everything
    python scripts/generate_test_vectors.py --skip-ita     # numpy-only tests
    python scripts/generate_test_vectors.py --ita-dir ~/ITA

Without ``--ita-dir`` the ITA repository is cloned into a temporary directory.
"""

from __future__ import annotations

import argparse
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

import numpy as np

# ITA sets these before emitting; without them numpy summarises long arrays
# with an ellipsis and the generated C is silently truncated.
np.set_printoptions(threshold = sys.maxsize, linewidth = np.inf)

REPO = Path(__file__).resolve().parent.parent
ITA_URL = "https://github.com/pulp-platform/ITA.git"

# Tests whose vectors come from an in-repo generate_vectors.py. Each generator
# already defaults its --output-* arguments to the committed paths, so calling
# it with no arguments regenerates in place.
NUMPY_TESTS = [
    "tests/chimera-convolve/snitchCluster/Add_L1",
    "tests/chimera-convolve/snitchCluster/iLayerNorm_L1",
    "tests/chimera-convolve/snitchCluster/iNoNorm_L1",
    "tests/chimera-convolve/snitchCluster/LayerScale_L1",
    "tests/chimera-convolve/snitchCluster/MatMul_L1",
    "tests/chimera-convolve/snitchCluster/ITA_Conv_L1",
    "tests/snitchCluster/matmul",
]

# ITA-generated tests. `shapes` maps the chimera file suffix to (S, E, P).
# `arrays` is the ordered set of declarations each file carries; the types must
# match what ITA_mem.h declares.
ITA_MATMUL_ARRAYS = [
    ("int8_t", "input_q", 1),
    ("int8_t", "input_Wq", 2),
    ("ita_int24_t", "input_Bq", 2),
    ("uint32_t", "requant_eps_mult", 2),
    ("uint32_t", "requant_right_shift", 2),
    ("int32_t", "requant_add", 2),
    ("int8_t", "golden_interm_Pq", 2),
]

ITA_SHA_ARRAYS = [
    ("int8_t", "input_q", 1),
    ("int8_t", "input_k", 1),
    ("int8_t", "input_Wq", 2),
    ("int8_t", "input_Wk", 2),
    ("int8_t", "input_Wv", 2),
    ("int8_t", "input_Wo", 2),
    ("ita_int24_t", "input_Bq", 2),
    ("ita_int24_t", "input_Bk", 2),
    ("ita_int24_t", "input_Bv", 2),
    ("ita_int24_t", "input_Bo", 2),
    ("uint32_t", "requant_eps_mult", 2),
    ("uint32_t", "requant_right_shift", 2),
    ("int32_t", "requant_add", 2),
    ("int8_t", "golden_interm_Pq", 2),
    ("int8_t", "golden_interm_Pk", 2),
    ("int8_t", "golden_interm_Pv", 2),
    ("int8_t", "golden_interm_attention", 2),
    ("int8_t", "golden_interm_head_output", 2),
    ("int8_t", "golden_output", 2),
]

MATMUL_SHAPES = {
    "64x64x64": (64, 64, 64),
    "64x128x64": (64, 128, 64),
    "64x256x64": (64, 256, 64),
    "64x512x64": (64, 512, 64),
    "128x512x64": (128, 512, 64),
    "192x512x64": (192, 512, 64),
    "256x512x64": (256, 512, 64),
    "512x512x64": (512, 512, 64),
}

SHA_SHAPES = {
    "S64_E128_P64": (64, 128, 64),
    "S128_E128_P64": (128, 128, 64),
    "S256_E384_P64": (256, 384, 64),
}

ITA_TESTS = [
    ("tests/chimera-convolve/snitchCluster/ITA_MatMul_L1", MATMUL_SHAPES, ITA_MATMUL_ARRAYS, "matmul"),
    ("tests/chimera-convolve/snitchCluster/ITA_MatMul_L2", MATMUL_SHAPES, ITA_MATMUL_ARRAYS, "matmul"),
    ("tests/chimera-convolve/snitchCluster/ITA_SHA_L2", SHA_SHAPES, ITA_SHA_ARRAYS, "sha"),
]

HEADER = """// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0
//
// GENERATED FILE -- DO NOT EDIT, AND DO NOT COMMIT.
// Produced by scripts/generate_test_vectors.py from ITA's testGenerator.py
//   python testGenerator.py -S {s} -E {e} -P {p} -B 1 -H 1 --export-snitch-cluster
// See docs on regenerating test vectors for the full flow.

#include "ITA_mem.h"

// clang-format off
"""


def run(cmd, cwd = None, env = None):
    proc = subprocess.run(cmd, cwd = cwd, env = env, capture_output = True, text = True)
    if proc.returncode != 0:
        sys.stderr.write(proc.stdout + proc.stderr)
        raise SystemExit(f"command failed ({proc.returncode}): {' '.join(map(str, cmd))}")
    return proc.stdout


def parse_c_arrays(text: str) -> dict:
    """Extract every ``name[...] = { ... }`` initialiser as a flat int array."""
    stripped = re.sub(r"//[^\n]*", "", text)
    out = {}
    for m in re.finditer(r"(\w+)\s*\[[^=]*=\s*\{", stripped):
        depth, i = 1, m.end()
        while depth and i < len(stripped):
            depth += (stripped[i] == "{") - (stripped[i] == "}")
            i += 1
        values = [int(x) for x in re.findall(r"-?\d+", stripped[m.end():i - 1])]
        if values:
            out[m.group(1)] = np.array(values, dtype = np.int64)
    return out


def format_array(values: np.ndarray) -> str:
    """Match the emitter ITA uses, so output is byte-identical to its export."""
    return np.array2string(values.flatten(), separator = ',', formatter = {'numpystr': lambda x: x})[1:-1]


def emit_ita_file(path: Path, arrays: dict, spec: list, shape: tuple) -> None:
    s, e, p = shape
    body = [HEADER.format(s = s, e = e, p = p)]
    for ctype, name, rank in spec:
        v = arrays[name]
        dims = f"[{v.size}]" if rank == 1 else f"[1][{v.size}]"
        inner = format_array(v)
        if rank == 1:
            body.append(f"const {ctype} {name}{dims} = {{\n{inner}\n}};\n")
        else:
            body.append(f"const {ctype} {name}{dims} = {{\n{{\n{inner}\n}}\n}};\n")
    body.append("// clang-format on\n")
    path.write_text("".join(body))


# ITA_dims_<shape>.h is the shape's dimension header. It is what
# chimera_select_shape() globs to discover the selectable shapes, and it carries
# the chimera-specific macros (N_CONTEXT, *_OPS, buffer sizes) on top of the
# dimensions PyITA reports.
DIMS_COMMON = """
// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _ITA_DIMS_INCLUDE_GUARD_
#define _ITA_DIMS_INCLUDE_GUARD_

#define HEADS 1
#define SEQUENCE_LENGTH {s}
#define EMBEDDING_SPACE {e}
#define PROJECTION_SPACE {p}
#define N_TILE_SEQUENCE_LENGTH {ts}
#define N_TILE_EMBEDDING_SPACE {te}
#define N_TILE_PROJECTION_SPACE {tp}
#define TILE_SIZE_SEQUENCE_LENGTH {m}
#define TILE_SIZE_EMBEDDING_SPACE {m}
#define TILE_SIZE_PROJECTION_SPACE {m}
#define N_CONTEXT 4
"""

DIMS_TAIL = """
#define MAX_TILE_SIZE \\
    (MAX(TILE_SIZE_SEQUENCE_LENGTH, MAX(TILE_SIZE_PROJECTION_SPACE, TILE_SIZE_EMBEDDING_SPACE)))
#define MAX_BUFFER_SIZE (MAX_TILE_SIZE * MAX_TILE_SIZE)
#define MAX_BUFFER_BIAS_SIZE (3 * MAX_TILE_SIZE)

#endif //_ITA_DIMS_INCLUDE_GUARD_"""

DIMS_MATMUL_OPS = """
// 1x MatMul SxExP + 1x Requant SxP
#define MAT_OPS \\
    (2ULL * SEQUENCE_LENGTH * EMBEDDING_SPACE * PROJECTION_SPACE + \\
     4ULL * SEQUENCE_LENGTH * PROJECTION_SPACE)
"""

DIMS_SHA_OPS = """
// 4x MatMul SxExP + 2xMatMul SxSxP + 4x Requant SxE
// + 1x Requant SxS + 1x Requant SxP + 1x Softmax SxS
#define SHA_OPS \\
    (2ULL * 4ULL * SEQUENCE_LENGTH * EMBEDDING_SPACE * PROJECTION_SPACE + \\
     2ULL * 2ULL * SEQUENCE_LENGTH * SEQUENCE_LENGTH * PROJECTION_SPACE + \\
     4ULL * 4ULL * SEQUENCE_LENGTH * EMBEDDING_SPACE + 4ULL * SEQUENCE_LENGTH * SEQUENCE_LENGTH + \\
     4ULL * SEQUENCE_LENGTH * PROJECTION_SPACE + 3ULL * SEQUENCE_LENGTH * SEQUENCE_LENGTH + \\
     SEQUENCE_LENGTH)
"""

ITA_M = 64


def emit_dims_file(path: Path, shape: tuple, ops: str) -> None:
    s, e, p = shape
    body = DIMS_COMMON.format(s = s, e = e, p = p, m = ITA_M, ts = s // ITA_M, te = e // ITA_M, tp = p // ITA_M)
    path.write_text(body + ops + DIMS_TAIL)


def ensure_ita(ita_dir: Path | None, workdir: Path) -> tuple[Path, Path]:
    """Return (ita repo path, python interpreter with ITA's deps installed)."""
    if ita_dir is None:
        ita_dir = workdir / "ITA"
        print(f"==> cloning ITA into {ita_dir}")
        run(["git", "clone", "--depth", "1", ITA_URL, str(ita_dir)])
    ita_dir = ita_dir.resolve()
    if not (ita_dir / "testGenerator.py").exists():
        raise SystemExit(f"{ita_dir} does not look like a checkout of {ITA_URL}")

    venv = workdir / "ita-venv"
    if not venv.exists():
        print("==> creating a virtualenv for ITA's generator")
        run([sys.executable, "-m", "venv", str(venv)])
        # testGenerator.py imports onnx at module scope even though the
        # snitch-cluster export does not use it.
        run([str(venv / "bin" / "pip"), "install", "-q", "numpy", "onnx"])
    return ita_dir, venv / "bin" / "python"


def generate_ita(ita_dir: Path | None, workdir: Path) -> int:
    ita, python = ensure_ita(ita_dir, workdir)
    wanted = {shape for _, shapes, _, _ in ITA_TESTS for shape in shapes.values()}
    exports = {}
    for (s, e, p) in sorted(wanted):
        print(f"==> ITA testGenerator: S={s} E={e} P={p}")
        run([
            str(python), "testGenerator.py", "-S",
            str(s), "-E",
            str(e), "-P",
            str(p), "-B", "1", "-H", "1", "--export-snitch-cluster"
        ],
            cwd = ita)
        export = ita / "simvectors" / f"data_S{s}_E{e}_P{p}_F64_H1_B1_Identity" \
                     / "snitch-cluster" / "mem_snitch_cluster.h"
        if not export.exists():
            raise SystemExit(f"ITA did not produce {export}")
        exports[(s, e, p)] = parse_c_arrays(export.read_text())

    written = 0
    for test, shapes, spec, kind in ITA_TESTS:
        ops = DIMS_SHA_OPS if kind == "sha" else DIMS_MATMUL_OPS
        for suffix, shape in shapes.items():
            arrays = exports[shape]
            missing = [n for _, n, _ in spec if n not in arrays]
            if missing:
                raise SystemExit(f"ITA export for {shape} lacks {missing}")
            emit_dims_file(REPO / test / "include" / f"ITA_dims_{suffix}.h", shape, ops)
            out = REPO / test / "src_cluster" / f"ITA_mem_{suffix}.c"
            emit_ita_file(out, arrays, spec, shape)
            written += 1
            print(f"    wrote {out.relative_to(REPO)}")
    return written


def generate_numpy() -> int:
    written = 0
    for test in NUMPY_TESTS:
        gen = REPO / test / "generate_vectors.py"
        if not gen.exists():
            print(f"    skip {test} (no generate_vectors.py)")
            continue
        print(f"==> {test}")
        run([sys.executable, str(gen)])
        written += 1
    return written


def main() -> None:
    ap = argparse.ArgumentParser(description = __doc__, formatter_class = argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--ita-dir",
                    type = Path,
                    default = None,
                    help = "Existing checkout of pulp-platform/ITA (cloned if omitted)")
    ap.add_argument("--skip-ita", action = "store_true", help = "Only run the numpy generators")
    ap.add_argument("--keep-workdir", action = "store_true", help = "Keep the temporary clone/virtualenv for reuse")
    args = ap.parse_args()

    workdir = Path(tempfile.mkdtemp(prefix = "chimera-vectors-"))
    try:
        n = generate_numpy()
        print(f"--- {n} numpy generators run")
        if not args.skip_ita:
            m = generate_ita(args.ita_dir, workdir)
            print(f"--- {m} ITA files written")
        print("done")
    finally:
        if args.keep_workdir:
            print(f"work directory kept at {workdir}")
        else:
            shutil.rmtree(workdir, ignore_errors = True)


if __name__ == "__main__":
    main()
