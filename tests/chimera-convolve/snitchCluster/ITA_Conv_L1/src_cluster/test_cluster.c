// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

// Include Application Headers
#include "test_cluster.h"
#include "test_host.h"

#include "ITA_mem.h"
#include "ITA_dims.h"
#include "ITA_types.h"
#include "ITA_utils.h"
#include "ITA_ctrl.h"

// Include Target Specific Headers
#include "soc.h"
#include "test.h"

// Include Driver Headers
#include "trampoline_snitchCluster.h"

// Include Runtime Headers
#include "snrt.h"

// #define DEBUG

#ifndef IM2COL_NUM_CORES
#define IM2COL_NUM_CORES 8
#endif

#define ITA_CHUNK_M TILE_SIZE_SEQUENCE_LENGTH
#define ITA_CHUNK_K TILE_SIZE_EMBEDDING_SPACE
#define ITA_CHUNK_N TILE_SIZE_PROJECTION_SPACE

// Shared pointers visible to all cluster cores (set by the DM core).
SNRT_CLUSTER_L1_ZERO(static int8_t *g_input_l1);
SNRT_CLUSTER_L1_ZERO(static int8_t *g_kernel_padded_l1);
SNRT_CLUSTER_L1_ZERO(static int8_t *g_im2col_tile[2]);
SNRT_CLUSTER_L1_ZERO(static int8_t *g_output_tile[2]);
SNRT_CLUSTER_L1_ZERO(static int8_t *g_output_l1);

// Generate one IM2COL tile; each core processes a subset of rows.
// Assumes input_ptr is laid out as NCHW with N=1 (i.e., CHW in memory).
static inline void im2col_tile_parallel(const int8_t *input_ptr, int8_t *dst, int tile_m_start,
                                        int tile_k_start, int workers, int worker_id) {
    const int rows = ITA_CHUNK_M;
    const int k_block = ITA_CHUNK_K;

    for (int local_m = worker_id; local_m < rows; local_m += workers) {
        const int out_idx = tile_m_start + local_m;
        const int oh = out_idx / OUTPUT_W;
        const int ow = out_idx - oh * OUTPUT_W;

        for (int k_inner = 0; k_inner < k_block; ++k_inner) {
            const int k_global = tile_k_start + k_inner;
            const int kh = k_global / KERNEL_W;
            const int kw = k_global % KERNEL_W;

            const int ih = oh + kh;
            const int iw = ow + kw;
            dst[local_m * k_block + k_inner] = input_ptr[ih * INPUT_W + iw];
        }
    }
}

static inline void col2im_tile_parallel(const int8_t *src, int8_t *output_ptr, int tile_m_start,
                                        int tile_n_start, int workers, int worker_id) {
    const int rows = ITA_CHUNK_M;
    const int n_block = ITA_CHUNK_N;

    for (int local_m = worker_id; local_m < rows; local_m += workers) {
        const int out_idx = tile_m_start + local_m;
        const int oh = out_idx / OUTPUT_W;
        const int ow = out_idx - oh * OUTPUT_W;

        for (int n_inner = 0; n_inner < n_block; ++n_inner) {
            const int n_global = tile_n_start + n_inner;
            output_ptr[(oh * OUTPUT_W + ow) * OUTPUT_C + n_global] =
                src[local_m * n_block + n_inner];
        }
    }
}

// Copy the computed output tile (only the valid output channels) back to the final output tensor.
static inline void writeback_output_tile(const int8_t *tile_ptr, int m_tile_idx, int8_t *output) {
    const int rows = ITA_CHUNK_M;
    const int valid_cols = OUTPUT_C;

    for (int local_m = 0; local_m < rows; ++local_m) {
        const int out_idx = m_tile_idx * ITA_CHUNK_M + local_m;
        const int oh = out_idx / OUTPUT_W;
        const int ow = out_idx - oh * OUTPUT_W;

        for (int n_inner = 0; n_inner < valid_cols; ++n_inner) {
            output[(n_inner * OUTPUT_H * OUTPUT_W) + (oh * OUTPUT_W + ow)] =
                tile_ptr[local_m * ITA_CHUNK_N + n_inner];
        }
    }
}

static inline void dump_matrix_s8(const int8_t *matrix, uint32_t rows, uint32_t cols) {
    for (uint32_t r = 0; r < rows; r++) {
        for (uint32_t c = 0; c < cols; c++) {
            snrt_printf("%4d ", matrix[r * cols + c]);
        }
        snrt_printf("\r\n");
    }
}

void PrintMatrix_s8_NHWC(int8_t const *__restrict__ pSrcA, uint32_t N, uint32_t C, uint32_t H,
                         uint32_t W, int32_t offset) {
    for (uint32_t n = 0; n < N; n++) {
        if (N > 0) snrt_printf("[\r\n");

        for (uint32_t c = 0; c < C; c++) {
            if (N > 0) {
                snrt_printf("  [\r\n  ");
            } else if (C > 0) {
                snrt_printf("[\r\n");
            }
            for (uint32_t h = 0; h < H; h++) {
                for (uint32_t w = 0; w < W; w++) {
                    snrt_printf("%4d ",
                                (int8_t)(pSrcA[n * C * H * W + h * C * W + w * C + c] + offset));
                }

                if (N > 0) {
                    snrt_printf("\r\n  ");
                } else {
                    snrt_printf("\r\n");
                }
            }
            if (C > 0) snrt_printf("]\r\n");
        }

        if (N > 0) snrt_printf("]\r\n");
    }
}

void PrintMatrix_s8_NHCW(int8_t const *__restrict__ pSrcA, uint32_t N, uint32_t C, uint32_t H,
                         uint32_t W, int32_t offset) {
    for (uint32_t n = 0; n < N; n++) {
        if (N > 0) snrt_printf("[\r\n");

        for (uint32_t c = 0; c < C; c++) {
            if (N > 0) {
                snrt_printf("  [\r\n  ");
            } else if (C > 0) {
                snrt_printf("[\r\n");
            }
            for (uint32_t h = 0; h < H; h++) {
                for (uint32_t w = 0; w < W; w++) {
                    snrt_printf("%4d ",
                                (int8_t)(pSrcA[n * C * H * W + c * H * W + h * W + w] + offset));
                }

                if (N > 0) {
                    snrt_printf("\r\n  ");
                } else {
                    snrt_printf("\r\n");
                }
            }
            if (C > 0) snrt_printf("]\r\n");
        }

        if (N > 0) snrt_printf("]\r\n");
    }
}

void PrintMatrix_s8_NCHW(int8_t const *__restrict__ pSrcA, uint32_t N, uint32_t C, uint32_t H,
                         uint32_t W, int32_t offset) {
    for (uint32_t n = 0; n < N; n++) {
        if (N > 0) snrt_printf("[\r\n");

        for (uint32_t c = 0; c < C; c++) {
            if (N > 0) {
                snrt_printf("  [\r\n  ");
            } else if (C > 0) {
                snrt_printf("[\r\n");
            }
            for (uint32_t h = 0; h < H; h++) {
                for (uint32_t w = 0; w < W; w++) {
                    snrt_printf("%4d ",
                                (int8_t)(pSrcA[n * C * H * W + c * H * W + h * W + w] + offset));
                }

                if (N > 0) {
                    snrt_printf("\r\n  ");
                } else {
                    snrt_printf("\r\n");
                }
            }
            if (C > 0) snrt_printf("]\r\n");
        }

        if (N > 0) snrt_printf("]\r\n");
    }
}

/**
 * @brief Interrupt handler for the cluster, which clears the interrupt flag for the current hart.
 *
 * @warning Stack, thread and global pointer might not yet be set up!
 */
__attribute__((naked)) void clusterInterruptHandler() {
    _SET_CLUSTER_BUSY();
    _SETUP_GP();

    asm volatile(
        // Load mhartid CSR into t0
        "csrr t0, mhartid\n"

        // Load clint base address into t1
        "la t1, __base_clint\n"

        // Calculate the interrupt target address: t1 = t1 + (t0 * 4)
        "slli t0, t0, 2\n"
        "add t1, t1, t0\n"
        // Store 0 to the interrupt target address
        "sw zero, 0(t1)\n"
        "ret"
        :            // No outputs
        :            // No inputs
        : "t0", "t1" // Declare clobbered registers
    );
}

/**
 * @brief Convolution with IM2COL parallelized over compute cores. IM2COL tiles are prepared while
 * the ITA accelerator processes the previous tile (ping-pong buffering).
 */
int32_t ita_matmul_l1_test(void *args) {
    int32_t tot_err = 0;

    test_cluster_args_t *test_args = (test_cluster_args_t *)args;
    test_cluster_result_t *test_retVal = (test_cluster_result_t *)(test_args->result);

    snrt_init();
    const int n_tiles = N_TILE_SEQUENCE_LENGTH;
    const int k_tiles = N_TILE_EMBEDDING_SPACE;
    const int m_tiles = N_TILE_PROJECTION_SPACE;

    const int total_tiles = m_tiles * k_tiles * n_tiles;
    const int im2col_workers = MIN(IM2COL_NUM_CORES, (int)snrt_cluster_compute_core_num());

    // Early exit if tiling assumptions are not met (no partial tiles handled).
    if ((IM2COL_N % ITA_CHUNK_K) != 0 || OUTPUT_C > ITA_CHUNK_N || m_tiles == 0 || k_tiles == 0) {
        if (snrt_is_dm_core()) {
            printf("Unsupported tensor shape for tiled IM2COL (requires full 64x64 tiles)\n");
        }
        snrt_cluster_hw_barrier();
        return -1;
    }

    if (snrt_is_dm_core()) {
        printf("Starting ITA Convolution execution (%ux%ux%u input, %ux%ux%u kernel) for %d "
               "iterations\n",
               INPUT_C, INPUT_H, INPUT_W, KERNEL_C, KERNEL_H, KERNEL_W, test_args->repetitions);

        // L1 buffers
        g_input_l1 = snrt_l1_alloc(INPUT_C * INPUT_H * INPUT_W);
        const int8_t *kernel_l1 = snrt_l1_alloc(OUTPUT_C * INPUT_C * KERNEL_H * KERNEL_W);
        g_kernel_padded_l1 = snrt_l1_alloc(ITA_CHUNK_K * ITA_CHUNK_N);
        g_im2col_tile[0] = snrt_l1_alloc(ITA_CHUNK_M * ITA_CHUNK_K);
        g_im2col_tile[1] = snrt_l1_alloc(ITA_CHUNK_M * ITA_CHUNK_K);
        g_output_tile[0] = snrt_l1_alloc(ITA_CHUNK_M * ITA_CHUNK_N);
        g_output_tile[1] = snrt_l1_alloc(ITA_CHUNK_M * ITA_CHUNK_N);
        g_output_l1 = snrt_l1_alloc(OUTPUT_C * OUTPUT_H * OUTPUT_W);

        snrt_dma_start_1d((void *)g_input_l1, (void *)input, INPUT_C * INPUT_H * INPUT_W);
        snrt_dma_start_1d((void *)kernel_l1, (void *)kernel,
                          OUTPUT_C * INPUT_C * KERNEL_H * KERNEL_W);
        snrt_dma_start_1d((void *)g_kernel_padded_l1, (void *)snrt_zero_memory_ptr(),
                          ITA_CHUNK_K * ITA_CHUNK_N);
        for (int c = 0; c < OUTPUT_C * INPUT_C; ++c) {
            snrt_dma_start_1d((void *)(g_kernel_padded_l1 + c * ITA_CHUNK_N),
                              (void *)(kernel_l1 + c * KERNEL_H * KERNEL_W), KERNEL_H * KERNEL_W);
        }
        snrt_dma_wait_all();

        // Properly reset and configure ITA contexts.
        ita_soft_clear();
        ita_acquire_job();
        for (int i = 0; i < N_CONTEXT; i++) {
            ita_set_layers(ITA_LAYER(LINEAR, IDENTITY));
            ita_set_tiles(ITA_TILES(m_tiles, k_tiles, n_tiles));
            ita_write_rqs_params(requant_eps_mult[0][0], requant_eps_mult[0][1],
                                 requant_right_shift[0][0], requant_right_shift[0][1],
                                 requant_add[0][0], requant_add[0][1]);
            ita_commit();
        }
    }

    snrt_cluster_hw_barrier();

    uint32_t start_cycles = 0, end_cycles = 0;
    uint32_t start_instructions = 0, end_instructions = 0;

    start_cycles = snrt_mcycle();
    start_instructions = snrt_minstret();

    for (int rep = 0; rep < test_args->repetitions; ++rep) {
        if (snrt_is_dm_core()) {
            ita_soft_clear_keep_regs();
            ita_acquire_job();
        }

        snrt_cluster_hw_barrier();

        int last_buf = -1;
        int last_m_tile = -1;

        for (int tile = 0; tile < total_tiles; ++tile) {
#ifdef DEBUG
            if (snrt_is_dm_core()) {
                printf("---------------------------------------------------------------------------"
                       "-----\n");
            }
#endif
            const int buf_idx = tile & 1;
            const int m_tile_idx = tile / k_tiles;
            const int k_tile_idx = tile - m_tile_idx * k_tiles;

            if (snrt_is_dm_core() && (tile > 0) && (tile % N_TILE_SEQUENCE_LENGTH == 0)) {
                ita_wait_job();
#ifdef DEBUG
                printf("ITA Output Tile (Tile %d, M tile %d) with shape (%ux%u):\n", tile - 1,
                       last_m_tile, ITA_CHUNK_M, ITA_CHUNK_N);
                PrintMatrix_s8_NCHW(g_output_tile[last_buf], 1, 1, ITA_CHUNK_M, ITA_CHUNK_N, 0);
#endif
                writeback_output_tile(g_output_tile[last_buf], last_m_tile, g_output_l1);
            }
#ifdef DEBUG
            if (snrt_is_dm_core()) {
                // Print kernel
                printf("Kernel Matrix (Tile %d, K tile %d) with shape (%ux%ux%u):\n", tile,
                       k_tile_idx, INPUT_C, ITA_CHUNK_K, ITA_CHUNK_N);
                PrintMatrix_s8_NCHW(g_kernel_padded_l1 + k_tile_idx * (ITA_CHUNK_K * ITA_CHUNK_N),
                                    1, 1, ITA_CHUNK_K, ITA_CHUNK_N, 0);
            }
#endif

            if (snrt_is_compute_core() && (snrt_cluster_core_idx() < im2col_workers)) {
                im2col_tile_parallel(g_input_l1, g_im2col_tile[buf_idx], m_tile_idx * ITA_CHUNK_M,
                                     k_tile_idx * ITA_CHUNK_K, im2col_workers,
                                     snrt_cluster_core_idx());
            }
#ifdef DEBUG
            if (snrt_is_dm_core()) {
                printf("IM2COL Matrix (Tile %d, M tile %d, K tile %d) with shape (%ux%u):\n", tile,
                       m_tile_idx, k_tile_idx, ITA_CHUNK_M, ITA_CHUNK_N);
                PrintMatrix_s8_NCHW(g_im2col_tile[buf_idx], 1, 1, ITA_CHUNK_M, ITA_CHUNK_N, 0);
            }
#endif

            snrt_cluster_hw_barrier();
            if (snrt_is_dm_core()) {
                const int8_t *weight_cur =
                    g_kernel_padded_l1 + k_tile_idx * (ITA_CHUNK_K * ITA_CHUNK_N);
                const int8_t *weight_next = (k_tile_idx + 1 < k_tiles)
                                                ? (weight_cur + ITA_CHUNK_K * ITA_CHUNK_N)
                                                : weight_cur;

                ita_set_addresses((uint32_t)g_im2col_tile[buf_idx], (uint32_t)weight_cur,
                                  (uint32_t)weight_next, (uint32_t)0,
                                  (uint32_t)g_output_tile[buf_idx]);
                ita_set_flags((uint32_t)ITA_FLAGS(tile == 0, (k_tile_idx + 1) < k_tiles, 1, 0,
                                                  (k_tile_idx + 1) < k_tiles));
                ita_trigger();
            }

            last_buf = buf_idx;
            last_m_tile = m_tile_idx;
        }

        if (snrt_is_dm_core()) {
            ita_wait_job();
            if (last_buf >= 0) {
#ifdef DEBUG
                printf("ITA Output Tile (Tile %d, M tile %d) with shape (%ux%u):\n",
                       total_tiles - 1, last_m_tile, ITA_CHUNK_M, ITA_CHUNK_N);
                PrintMatrix_s8_NCHW(g_output_tile[last_buf], 1, 1, ITA_CHUNK_M, ITA_CHUNK_N, 0);
#endif
                writeback_output_tile(g_output_tile[last_buf], last_m_tile, g_output_l1);
            }
        }

        snrt_cluster_hw_barrier();
    }
    end_cycles = snrt_mcycle();
    end_instructions = snrt_minstret();

    if (snrt_is_dm_core()) {

        // printf("Dumping final output tensor:\n");
        // dump_matrix_s8(g_output_l1, OUTPUT_C, OUTPUT_H * OUTPUT_W);

        printf("Output Matrix with shape (%ux%ux%u):\n", OUTPUT_C, OUTPUT_H, OUTPUT_W);
        PrintMatrix_s8_NCHW(g_output_l1, 1, OUTPUT_C, OUTPUT_H, OUTPUT_W, 0);

        printf("ITA Conv cycles = %u\n", end_cycles - start_cycles);
        printf("ITA Conv instructions = %u\n", end_instructions - start_instructions);

        float ops = (float)CONV_OPS * test_args->repetitions;
        float ops_per_cycle = ops / (float)(end_cycles - start_cycles);

        tot_err =
            check(1, (uint8_t *)g_output_l1, (uint8_t *)golden, OUTPUT_H * OUTPUT_W * OUTPUT_C,
                  g_input_l1, 1 * OUTPUT_H * OUTPUT_W * OUTPUT_C);

        if (tot_err != 0) {
            printf("Test failed with %d errors\r\n", tot_err);
        }

        test_retVal->errors = tot_err;
        test_retVal->ops_per_cycle = (uint32_t)(ops_per_cycle * 1e6);
        test_retVal->runtime_cycles = end_cycles - start_cycles;
    }

    snrt_cluster_hw_barrier();

    return tot_err << 1;
}
