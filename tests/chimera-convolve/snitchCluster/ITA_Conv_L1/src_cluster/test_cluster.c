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

#define ITA_CHUNK_M TILE_SIZE_SEQUENCE_LENGTH
#define ITA_CHUNK_K TILE_SIZE_EMBEDDING_SPACE
#define ITA_CHUNK_N TILE_SIZE_PROJECTION_SPACE

// Shared pointers visible to all cluster cores (set by the DM core).
SNRT_CLUSTER_L1_ZERO(static int8_t *g_input_l1);
SNRT_CLUSTER_L1_ZERO(static int8_t *g_kernel_padded_l1);
SNRT_CLUSTER_L1_ZERO(static int8_t *g_im2col_tile[2]);
SNRT_CLUSTER_L1_ZERO(static int8_t *g_output_tile[2]);
SNRT_CLUSTER_L1_ZERO(static int8_t *g_output_l1);

SNRT_CLUSTER_L1_ZERO(static uint32_t tot_err);

// Generate one IM2COL tile
// Assumes input_ptr is laid out as NCHW with N=1 (i.e., CHW in memory).
static inline void im2col_tile(const int8_t *input_ptr, int8_t *dst, int tile_m_start) {
    const int rows = ITA_CHUNK_M;
    const int k_block = ITA_CHUNK_K;
    const size_t row_bytes = KERNEL_W;
    const size_t dst_stride = KERNEL_W;
    const size_t src_stride = INPUT_W;
    const size_t repeat = KERNEL_H;

    for (int i = 0; i < rows; ++i) {
        const int out_idx = tile_m_start + i;
        const int oh = out_idx / OUTPUT_W;
        const int ow = out_idx - oh * OUTPUT_W;

        int8_t *dst_row = dst + i * k_block;
        const int8_t *src_row = input_ptr + (oh * INPUT_W + ow);

        // DMA copies the 2D patch (KERNEL_H x KERNEL_W) into a contiguous row.
        snrt_dma_start_2d(dst_row, src_row, row_bytes, dst_stride, src_stride, repeat);
    }
}

// Copy a computed output tile back into the final tensor using 2D DMA.
// DM-core only; no parallelism needed.
static inline void col2im_tile(const int8_t *src, int8_t *output_ptr, int tile_m_start,
                               int tile_n_start) {
    const int rows = ITA_CHUNK_M;
    const int n_block = ITA_CHUNK_N;

    const size_t row_bytes = n_block;
    const size_t dst_stride = OUTPUT_C;
    const size_t src_stride = n_block;
    const size_t repeat = rows;

    const int base_pos = tile_m_start; // Linear spatial index for first row in this tile
    int8_t *dst_row0 = output_ptr + base_pos * OUTPUT_C + tile_n_start;
    const int8_t *src_row0 = src; // row 0 starts at src + 0 * n_block

    snrt_dma_start_2d(dst_row0, src_row0, row_bytes, dst_stride, src_stride, repeat);
    snrt_dma_wait_all();
}

// Copy the computed output tile (only the valid output channels) back to the final output tensor.
static inline void writeback_output_tile(const int8_t *tile_ptr, int m_tile_idx, int8_t *output) {
    const int rows = ITA_CHUNK_M;
    const int valid_cols = OUTPUT_C;

    const size_t row_bytes = 1;
    const size_t dst_stride = 1;
    const size_t src_stride = ITA_CHUNK_N;
    const size_t repeat = rows;

    const int base_pos = m_tile_idx * ITA_CHUNK_M; // Linear spatial index for first row

    for (int n_inner = 0; n_inner < valid_cols; ++n_inner) {
        int8_t *dst_row0 = output + n_inner * OUTPUT_H * OUTPUT_W + base_pos;
        const int8_t *src_row0 = tile_ptr + n_inner;
        snrt_dma_start_2d(dst_row0, src_row0, row_bytes, dst_stride, src_stride, repeat);
    }
    snrt_dma_wait_all();
}

static inline void dump_matrix_s8(const int8_t *matrix, uint32_t rows, uint32_t cols) {
    for (uint32_t r = 0; r < rows; r++) {
        for (uint32_t c = 0; c < cols; c++) {
            snrt_printf("%4d ", matrix[r * cols + c]);
        }
        snrt_printf("\r\n");
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

void ita_set_tiles_fix(uint8_t m_tiles, uint8_t k_tiles, uint8_t n_tiles) {
    const int max_ita_tiles = 8;
#ifdef DEBUG
    printf("Configured ITA for max tiles (%d x %d x %d)\n", max_ita_tiles, max_ita_tiles,
           max_ita_tiles);
    printf("Total tiles to process (%d x %d x %d)\n", m_tiles, k_tiles, n_tiles);
#endif
    for (int i = 0; i < N_CONTEXT; i++) {
        // Each batch processes min(max_ita_tiles, remaining_tiles) in each dimension
        ita_set_tiles(ITA_TILES(MIN(max_ita_tiles, m_tiles), MIN(max_ita_tiles, k_tiles),
                                MIN(max_ita_tiles, n_tiles)));
    }
}

/**
 * @brief Convolution with IM2COL parallelized over compute cores. IM2COL tiles are prepared while
 * the ITA accelerator processes the previous tile (ping-pong buffering).
 */
int32_t ita_conv_l1_test(void *args) {
    test_cluster_args_t *test_args = (test_cluster_args_t *)args;
    test_cluster_result_t *test_retVal = (test_cluster_result_t *)(test_args->result);

    snrt_init();
    // IM2COL MatMul: (M x K) * (K x N) = (M x N)
    // M = spatial dim (OUTPUT_H * OUTPUT_W)
    // K = embedding dim (KERNEL_H * KERNEL_W * INPUT_C)
    // N = projection dim (OUTPUT_C)
    const int m_tiles_total = N_TILE_SEQUENCE_LENGTH;
    const int k_tiles_total = N_TILE_EMBEDDING_SPACE;
    const int n_tiles_total = N_TILE_PROJECTION_SPACE;

    // ITA can process at most 8 tiles per dimension, so batch larger dimensions
    const int max_ita_tiles = 8;
    const int m_batches = (m_tiles_total + max_ita_tiles - 1) / max_ita_tiles;
    const int k_batches = (k_tiles_total + max_ita_tiles - 1) / max_ita_tiles;
    const int n_batches = (n_tiles_total + max_ita_tiles - 1) / max_ita_tiles;

    // Early exit if no tiles to process.
    if (m_tiles_total == 0 || k_tiles_total == 0 || n_tiles_total == 0) {
        if (snrt_is_dm_core()) {
            printf("Error: No tiles to process (m=%d, k=%d, n=%d)\n", m_tiles_total, k_tiles_total,
                   n_tiles_total);
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
            // Each batch processes min(max_ita_tiles, remaining_tiles) in each dimension
            ita_set_tiles(ITA_TILES(MIN(max_ita_tiles, m_tiles_total),
                                    MIN(max_ita_tiles, k_tiles_total),
                                    MIN(max_ita_tiles, n_tiles_total)));
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

        if (snrt_is_dm_core()) {
            // Prefetch IM2COL for the first tile so we can overlap DMA with ITA compute.
            im2col_tile(g_input_l1, g_im2col_tile[0], 0);
        }

        // Batch over M, K, N dimensions (ITA can handle max 8 tiles per dimension)
        for (int m_batch = 0; m_batch < m_batches; ++m_batch) {
            for (int k_batch = 0; k_batch < k_batches; ++k_batch) {
                for (int n_batch = 0; n_batch < n_batches; ++n_batch) {
                    // Compute the tile range for this batch
                    int m_start = m_batch * max_ita_tiles;
                    int m_end = (m_batch + 1) * max_ita_tiles < m_tiles_total
                                    ? (m_batch + 1) * max_ita_tiles
                                    : m_tiles_total;
                    int k_start = k_batch * max_ita_tiles;
                    int k_end = (k_batch + 1) * max_ita_tiles < k_tiles_total
                                    ? (k_batch + 1) * max_ita_tiles
                                    : k_tiles_total;
                    int n_start = n_batch * max_ita_tiles;
                    int n_end = (n_batch + 1) * max_ita_tiles < n_tiles_total
                                    ? (n_batch + 1) * max_ita_tiles
                                    : n_tiles_total;

                    int m_batch_tiles = m_end - m_start;
                    int k_batch_tiles = k_end - k_start;
                    int n_batch_tiles = n_end - n_start;

#ifdef DEBUG
                    if (snrt_is_dm_core()) {
                        printf("Processing batch M[%d-%d] K[%d-%d] N[%d-%d] with (%d x %d x %d) "
                               "tiles\n",
                               m_start, m_end - 1, k_start, k_end - 1, n_start, n_end - 1,
                               m_batch_tiles, k_batch_tiles, n_batch_tiles);
                    }
#endif

                    // Configure ITA for this batch's tile counts
                    if (snrt_is_dm_core()) {
                        ita_set_tiles_fix(m_batch_tiles, k_batch_tiles, n_batch_tiles);
                    }
                    snrt_cluster_hw_barrier();

                    // Iterate through all tiles in this batch
                    int batch_tile = 0;
                    for (int m = m_start; m < m_end; ++m) {
                        for (int k = k_start; k < k_end; ++k) {
                            for (int n = n_start; n < n_end; ++n) {
#ifdef DEBUG
                                if (snrt_is_dm_core()) {
                                    printf("-------------------------------------------------------"
                                           "------------"
                                           "---------\n");
                                }
#endif
                                const int buf_idx = batch_tile & 1;
                                const int m_tile_idx = m;
                                const int k_tile_idx = k;
                                const int n_tile_idx = n;
#ifdef DEBUG
                                if (snrt_is_dm_core()) {
                                    printf(
                                        "Tile (batch_tile %d): M tile %d, K tile %d, N tile %d\n",
                                        batch_tile, m_tile_idx, k_tile_idx, n_tile_idx);
                                }
#endif

                                if (snrt_is_dm_core() && (batch_tile > 0) &&
                                    (batch_tile % k_batch_tiles == 0)) {
#ifdef DEBUG
                                    printf("Waiting for ITA job to complete...\n");
#endif
                                    ita_wait_job();
#ifdef DEBUG
                                    printf("ITA Output Tile (batch_tile %d, M tile %d) with shape "
                                           "(%ux%u):\n",
                                           batch_tile - 1, last_m_tile, ITA_CHUNK_M, ITA_CHUNK_N);
                                    PrintMatrix_s8_NCHW(g_output_tile[last_buf], 1, 1, ITA_CHUNK_M,
                                                        ITA_CHUNK_N, 0);
#endif
                                    writeback_output_tile(g_output_tile[last_buf], last_m_tile,
                                                          g_output_l1);
                                }
#ifdef DEBUG
                                if (snrt_is_dm_core()) {
                                    // Print kernel
                                    printf("Kernel Matrix (batch_tile %d, K tile %d) with shape "
                                           "(%ux%ux%u):\n",
                                           batch_tile, k_tile_idx, INPUT_C, ITA_CHUNK_K,
                                           ITA_CHUNK_N);
                                    PrintMatrix_s8_NCHW(g_kernel_padded_l1 +
                                                            k_tile_idx *
                                                                (ITA_CHUNK_K * ITA_CHUNK_N),
                                                        1, 1, ITA_CHUNK_K, ITA_CHUNK_N, 0);
                                }
#endif

                                if (snrt_is_dm_core()) {
                                    // Ensure IM2COL DMA for the current tile has finished before
                                    // programming ITA.
                                    snrt_dma_wait_all();

#ifdef DEBUG
                                    printf("IM2COL Tile (batch_tile %d, M tile %d) with shape "
                                           "(%ux%u):\n",
                                           batch_tile, m_tile_idx, ITA_CHUNK_M, ITA_CHUNK_K);
                                    PrintMatrix_s8_NCHW(g_im2col_tile[buf_idx], 1, 1, ITA_CHUNK_M,
                                                        ITA_CHUNK_K, 0);
#endif

                                    const int8_t *weight_cur =
                                        g_kernel_padded_l1 +
                                        k_tile_idx * (ITA_CHUNK_K * ITA_CHUNK_N);
                                    const int8_t *weight_next =
                                        (k_tile_idx + 1 < k_tiles_total)
                                            ? (weight_cur + ITA_CHUNK_K * ITA_CHUNK_N)
                                            : weight_cur;

                                    ita_set_addresses((uint32_t)g_im2col_tile[buf_idx],
                                                      (uint32_t)weight_cur, (uint32_t)weight_next,
                                                      (uint32_t)0,
                                                      (uint32_t)g_output_tile[buf_idx]);
                                    int is_first = (m == 0 && k == 0 && n == 0);
                                    int is_last =
                                        (m == m_tiles_total - 1 && k == k_tiles_total - 1 &&
                                         n == n_tiles_total - 1);
                                    int has_next_k = (k + 1 < k_tiles_total);
                                    int has_next_n = (n + 1 < n_tiles_total);

                                    ita_set_flags(
                                        (uint32_t)ITA_FLAGS(is_first, !is_last, 1, 0, has_next_k));
                                    ita_trigger();
#ifdef DEBUG
                                    printf(
                                        "ITA triggered for batch_tile %d (M tile %d, K tile %d, N "
                                        "tile %d)\n",
                                        batch_tile, m_tile_idx, k_tile_idx, n_tile_idx);
                                    printf(
                                        "  is_first=%d, is_last=%d, has_next_k=%d, has_next_n=%d\n",
                                        is_first, is_last, has_next_k, has_next_n);
#endif
                                    last_buf = buf_idx;
                                    last_m_tile = m_tile_idx;

                                    // Prefetch IM2COL for the next M tile while ITA processes the
                                    // current one.
                                    if (batch_tile + 1 <
                                        m_batch_tiles * k_batch_tiles * n_batch_tiles) {
                                        const int next_batch_tile = batch_tile + 1;
                                        const int next_buf = next_batch_tile & 1;
                                        const int next_m =
                                            m_start +
                                            (next_batch_tile / (k_batch_tiles * n_batch_tiles));
#ifdef DEBUG
                                        printf("Prefetching IM2COL for batch_tile %d (M tile %d)\n",
                                               next_batch_tile, next_m);
#endif
                                        im2col_tile(g_input_l1, g_im2col_tile[next_buf],
                                                    next_m * ITA_CHUNK_M);
                                    }
                                }
                                batch_tile++;
                            }
                        }
                    }
                }
            }
        }

        if (snrt_is_dm_core()) {
#ifdef DEBUG
            printf("Waiting for final ITA jobs to complete...\n");
#endif
            ita_wait_job();
            if (last_buf >= 0) {
#ifdef DEBUG
                printf("ITA Output Tile (final, M tile %d) with shape
                       (% ux % u) :\n ", last_m_tile,
                       ITA_CHUNK_M,
                       ITA_CHUNK_N);
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
#ifdef DEBUG
        printf("Dumping final output tensor:\n");
        dump_matrix_s8(g_output_l1, OUTPUT_C, OUTPUT_H * OUTPUT_W);

        printf("Output Matrix with shape (%ux%ux%u):\n", OUTPUT_C, OUTPUT_H, OUTPUT_W);
        PrintMatrix_s8_NCHW(g_output_l1, 1, OUTPUT_C, OUTPUT_H, OUTPUT_W, 0);
#endif

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
