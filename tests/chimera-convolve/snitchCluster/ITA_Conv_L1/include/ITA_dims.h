// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _ITA_DIMS_INCLUDE_GUARD_
#define _ITA_DIMS_INCLUDE_GUARD_

#define INPUT_H 15
#define INPUT_W INPUT_H
#define INPUT_C 1

#define KERNEL_H 8
#define KERNEL_W KERNEL_H
#define KERNEL_C 2

#define OUTPUT_H (INPUT_H - KERNEL_H + 1)
#define OUTPUT_W (INPUT_W - KERNEL_W + 1)
#define OUTPUT_C KERNEL_C

#define N_TILE_SEQUENCE_LENGTH 1
#define N_TILE_EMBEDDING_SPACE 1
#define N_TILE_PROJECTION_SPACE 1

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
#define CONV_OPS \
    ((2ULL * OUTPUT_H * OUTPUT_W * OUTPUT_C * KERNEL_H * KERNEL_W * INPUT_C) + \
     (4ULL * OUTPUT_H * OUTPUT_W * OUTPUT_C))

#define MAX_TILE_SIZE \
    (MAX(TILE_SIZE_SEQUENCE_LENGTH, MAX(TILE_SIZE_PROJECTION_SPACE, TILE_SIZE_EMBEDDING_SPACE)))
#define MAX_BUFFER_SIZE (MAX_TILE_SIZE * MAX_TILE_SIZE)
#define MAX_BUFFER_BIAS_SIZE (3 * MAX_TILE_SIZE)

#endif //_ITA_DIMS_INCLUDE_GUARD_
