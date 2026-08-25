#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0
"""
Generate deterministic input/kernel/golden fixtures for ITA_Conv_L1.
"""

from __future__ import annotations

import argparse
from pathlib import Path
from textwrap import dedent

import numpy as np

# Tensor dimensions (mirrors ITA_dims.h)
INPUT_H = (15 + 8)
INPUT_W = INPUT_H
INPUT_C = 1

KERNEL_H = 8
KERNEL_W = KERNEL_H
KERNEL_C = 2
OUTPUT_C = KERNEL_C

OUTPUT_H = INPUT_H - KERNEL_H + 1
OUTPUT_W = INPUT_W - KERNEL_W + 1

# Constants used for requantization (kept in sync with ITA_mem.c)
REQ_EPS_MULT = 16
REQ_RIGHT_SHIFT = 12
REQ_ADD = 0

# Set random seeds
np.random.seed(0)


def gen_input() -> np.ndarray:
    return np.random.randint(-128, 128, size = (INPUT_C, INPUT_H, INPUT_W), dtype = np.int8)


def gen_kernel() -> np.ndarray:
    return np.random.randint(-128, 128, size = (KERNEL_C, KERNEL_H, KERNEL_W), dtype = np.int8)


# Inputs are in CHW format OIHW
def conv(inp: np.ndarray, ker: np.ndarray) -> np.ndarray:
    """
    Perform a valid convolution of inp with ker.
    inp: [INPUT_C, INPUT_H, INPUT_W]
    ker: [OUTPUT_C, INPUT_C, KERNEL_H, KERNEL_W]
    returns: [OUTPUT_C, OUTPUT_H, OUTPUT_W]
    """
    in_transposed = inp.transpose((1, 2, 0)).astype(np.int32)  # from CHW to HWC
    ker_transposed = ker.transpose((2, 3, 1, 0)).astype(np.int32)  # from OIHW to HWIO
    out = np.zeros((OUTPUT_H, OUTPUT_W, OUTPUT_C), dtype = np.int32)
    for oh in range(OUTPUT_H):
        for ow in range(OUTPUT_W):
            patch = in_transposed[oh:oh + KERNEL_H, ow:ow + KERNEL_W, :]
            out[oh, ow] = np.tensordot(patch, ker_transposed, axes = ([0, 1, 2], [0, 1, 2]))

    out = out.transpose((2, 0, 1))  # back to CHW
    return out.astype(np.int32)


def rqs(matrix: np.ndarray, eps_mult: int, right_shift: int, add: int):
    """
    Requantizes the values in a 3D matrix with specified multipliers, right shifts, and additions.

    Args:
        matrix (np.ndarray): The input 3D matrix to be requantized.
        eps_mult (int): The multipliers for the elements.
        right_shift (int): The amounts to right shift the result.
        add (int): The values to add after shifting.

    Returns:
        np.ndarray: The requantized 3D matrix.

    Example:
        >>> matrix = np.array([[[50, 100], [150, 200]]])
        >>> eps_mult = [2]
        >>> right_shift = [1]
        >>> add = [1]
        >>> requantize3(matrix, eps_mult, right_shift, add)
        array([[[ 51, 101],
                [127, 127]]], dtype=int8)
    """

    # Cast up to int32 to avoid overflow
    eps_mult = np.array(eps_mult, dtype = np.uint32)
    right_shift = np.array(right_shift, dtype = np.uint32)
    add = np.array(add, dtype = np.int32)

    result = np.empty(matrix.shape, dtype = np.int8)
    for h_ind, heads in enumerate(matrix):
        for r_ind, row in enumerate(heads):
            for c_ind, element in enumerate(row):
                # shifted = ((eps_mult[h_ind] * element) >> right_shift[h_ind]) + add[h_ind]
                shifted = ((eps_mult * element) / 2**right_shift) + add
                shifted = np.floor(shifted + 0.5 + np.finfo(np.float32).eps)
                if shifted > 127:
                    result[h_ind, r_ind, c_ind] = 127
                elif shifted < -128:
                    result[h_ind, r_ind, c_ind] = -128
                else:
                    result[h_ind, r_ind, c_ind] = shifted.astype(np.int8)
    return result


def format_array(arr: np.ndarray, per_line: int = 12) -> str:
    vals = arr.flatten().tolist()
    return ",".join(str(int(x)) for x in vals)


def build_c_body(inp: np.ndarray, ker: np.ndarray, out: np.ndarray) -> str:
    return dedent(f"""
        // SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
        // SPDX-License-Identifier: Apache-2.0

        #include \"ITA_mem.h\"
        #include \"ITA_dims.h\"

        // clang-format off
        const int8_t input[{inp.size}] = {{
        {format_array(inp)}
        }};

        // Kernel flattened as [KERNEL_H * KERNEL_W * INPUT_C * OUTPUT_C]
        const int8_t kernel[{ker.size}] = {{
        {format_array(ker)}
        }};

        const int8_t golden[{out.size}] = {{
        {format_array(out)}
        }};

        const uint32_t requant_eps_mult[1][2] = {{
        {{
            0x{REQ_EPS_MULT:02x}{REQ_EPS_MULT:02x}{REQ_EPS_MULT:02x}{REQ_EPS_MULT:02x}, 0x{REQ_EPS_MULT:02x}{REQ_EPS_MULT:02x}
        }}
        }};

        const uint32_t requant_right_shift[1][2] = {{
        {{
            0x{REQ_RIGHT_SHIFT:02x}{REQ_RIGHT_SHIFT:02x}{REQ_RIGHT_SHIFT:02x}{REQ_RIGHT_SHIFT:02x}, 0x{REQ_RIGHT_SHIFT:02x}{REQ_RIGHT_SHIFT:02x}
        }}
        }};
        const int32_t requant_add[1][2] = {{
        {{
            0x{REQ_ADD:02x}{REQ_ADD:02x}{REQ_ADD:02x}{REQ_ADD:02x}, 0x{REQ_ADD:02x}{REQ_ADD:02x}
        }}
        }};
        // clang-format on
        """).strip() + "\n"


def build_mem_h_body(inp_size: int, ker_size: int, golden_size: int) -> str:
    return dedent(f"""
        // SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
        // SPDX-License-Identifier: Apache-2.0

        #ifndef _ITA_MEM_INCLUDE_GUARD_
        #define _ITA_MEM_INCLUDE_GUARD_

        #include <stdint.h>

        #include "ITA_types.h"
        #include "ITA_dims.h"

        // clang-format off
        extern const int8_t input[{inp_size}];
        extern const int8_t kernel[{ker_size}];
        extern const int8_t golden[{golden_size}];
        extern const uint32_t requant_eps_mult[1][2];
        extern const uint32_t requant_right_shift[1][2];
        extern const int32_t requant_add[1][2];

        // clang-format on

        #endif
        """).strip() + "\n"


def build_h_body() -> str:
    # IM2COL MatMul dimensions: (M x K) * (K x N) = (M x N)
    # M = spatial positions (OUTPUT_H * OUTPUT_W)
    # K = patch size (KERNEL_H * KERNEL_W * INPUT_C)
    # N = output channels (OUTPUT_C)
    seq_len = OUTPUT_H * OUTPUT_W  # M dimension
    emb_space = KERNEL_H * KERNEL_W * INPUT_C  # K dimension
    proj_space = OUTPUT_C  # N dimension

    # Calculate number of tiles (ceiling division to handle partial tiles)
    n_tile_seq = (seq_len + 63) // 64  # Tiles in M dimension
    n_tile_emb = (emb_space + 63) // 64  # Tiles in K dimension
    n_tile_proj = (proj_space + 63) // 64  # Tiles in N dimension

    return dedent(f"""
        // SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
        // SPDX-License-Identifier: Apache-2.0

        #ifndef _ITA_DIMS_INCLUDE_GUARD_
        #define _ITA_DIMS_INCLUDE_GUARD_

        #define INPUT_H {INPUT_H}
        #define INPUT_W INPUT_H
        #define INPUT_C {INPUT_C}

        #define KERNEL_H {KERNEL_H}
        #define KERNEL_W KERNEL_H
        #define KERNEL_C {KERNEL_C}

        #define OUTPUT_H (INPUT_H - KERNEL_H + 1)
        #define OUTPUT_W (INPUT_W - KERNEL_W + 1)
        #define OUTPUT_C KERNEL_C

        #define N_TILE_SEQUENCE_LENGTH {n_tile_seq}
        #define N_TILE_EMBEDDING_SPACE {n_tile_emb}
        #define N_TILE_PROJECTION_SPACE {n_tile_proj}

        // IM2COL Convolution is MxK * KxN = MxN
        #define IM2COL_N (OUTPUT_H * OUTPUT_W)
        #define IM2COL_K (KERNEL_H * KERNEL_W)
        #define IM2COL_M (OUTPUT_C)

        #define TILE_SIZE_SEQUENCE_LENGTH 64
        #define TILE_SIZE_EMBEDDING_SPACE 64
        #define TILE_SIZE_PROJECTION_SPACE 64
        #define N_CONTEXT 4

        // 1x IM2COL IwxIhxIc to Iw x Ih x (Kh x Kw x Ic) (only data movement)
        // 1x Conv Iw x Ih x Ic o Kh x Kw x Kc
        // 1x Requant Oh x Ow x Oc
        #define CONV_OPS \\
            ((2ULL * OUTPUT_H * OUTPUT_W * OUTPUT_C * KERNEL_H * KERNEL_W * INPUT_C) + \\
             (4ULL * OUTPUT_H * OUTPUT_W * OUTPUT_C))

        #define MAX_TILE_SIZE \\
            (MAX(TILE_SIZE_SEQUENCE_LENGTH, MAX(TILE_SIZE_PROJECTION_SPACE, TILE_SIZE_EMBEDDING_SPACE)))
        #define MAX_BUFFER_SIZE (MAX_TILE_SIZE * MAX_TILE_SIZE)
        #define MAX_BUFFER_BIAS_SIZE (3 * MAX_TILE_SIZE)

        #endif //_ITA_DIMS_INCLUDE_GUARD_
        """).strip() + "\n"


def main() -> None:
    parser = argparse.ArgumentParser(description = "Generate ITA_mem.c fixtures")
    default_output_mem = Path(__file__).parent / "src_cluster" / "ITA_mem.c"
    parser.add_argument(
        "--output-mem",
        type = Path,
        default = default_output_mem,
        help = f"Path to write the generated C data file (default: {default_output_mem})",
    )
    default_output_dims = Path(__file__).parent / "include" / "ITA_dims.h"
    parser.add_argument(
        "--output-dims",
        type = Path,
        default = default_output_dims,
        help = f"Path to write the generated dimensions header file (default: {default_output_dims})",
    )
    default_output_mem_h = Path(__file__).parent / "include" / "ITA_mem.h"
    parser.add_argument(
        "--output-mem-h",
        type = Path,
        default = default_output_mem_h,
        help = f"Path to write the generated memory header file (default: {default_output_mem_h})",
    )
    args = parser.parse_args()

    inp = gen_input()
    print("Input Shape:", inp.shape)
    ker = gen_kernel()
    print("Kernel Shape:", ker.shape)
    # Broadcast from OHW to OIHW
    ker_broadcasted = np.tile(ker[:, np.newaxis, :, :], (1, INPUT_C, 1, 1))
    print("Broadcasted Kernel Shape:", ker_broadcasted.shape)
    out = conv(inp, ker_broadcasted)
    out_rq = rqs(out, REQ_EPS_MULT, REQ_RIGHT_SHIFT, REQ_ADD)

    # Calculate the IM2COL transformed input size for verification
    im2col_n = KERNEL_H * KERNEL_W * INPUT_C
    im2col_m = OUTPUT_H * OUTPUT_W

    # Zero-pad the golden output to match tile boundaries
    # The output is in CHW format, need to pad to (OUTPUT_C, padded_H*W)
    n_tile_seq = (im2col_m + 63) // 64
    padded_seq_len = n_tile_seq * 64

    if padded_seq_len > im2col_m:
        # Need to pad the spatial dimensions
        # Reshape from (C, H, W) to (C, H*W), pad, then keep as 1D for storage
        out_rq_flat = out_rq.reshape(OUTPUT_C, OUTPUT_H * OUTPUT_W)
        out_rq_padded = np.zeros((OUTPUT_C, padded_seq_len), dtype = np.int8)
        out_rq_padded[:, :OUTPUT_H * OUTPUT_W] = out_rq_flat
        out_rq = out_rq_padded.reshape(OUTPUT_C, -1)

    print(f"Generated input with shape {inp.shape}")
    print(f"IM2COL transformed input shape: ({im2col_m}, {im2col_n})")
    print(f"Padded output shape: {out_rq.shape} (spatial dim padded to {padded_seq_len})")

    body_mem = build_c_body(inp, ker_broadcasted, out_rq)
    args.output_mem.write_text(body_mem)

    body_dims = build_h_body()
    args.output_dims.write_text(body_dims)

    body_mem_h = build_mem_h_body(inp.size, ker_broadcasted.size, out_rq.size)
    args.output_mem_h.write_text(body_mem_h)
    # print(
    #     f"Wrote {args.output_mem} with input {inp.size}, kernel {ker.size}, golden {out.size} entries"
    # )

    # Pretty print all matrices in CHW format for verification (first transpose them)
    np.set_printoptions(linewidth = 200, threshold = 10000, formatter = {"int": "{:4}".format})
    print("\nInput:")
    print(inp)
    print("\nKernel:")
    print(ker_broadcasted)
    print("\nGolden:")
    print(out_rq)


if __name__ == "__main__":
    main()
