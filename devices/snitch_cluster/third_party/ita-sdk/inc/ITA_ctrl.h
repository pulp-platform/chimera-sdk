// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _ITA_CTRL_INCLUDE_GUARD_
#define _ITA_CTRL_INCLUDE_GUARD_

// Include Standard Libraries
#include <stdint.h>

// Include Application Headers
#include "ITA_mem.h"
#include "ITA_types.h"

// Include Target Specific Headers
#include "soc.h"
#include "cluster_4.h"

// Include Driver Headers

// Include Runtime Headers
#include "snrt.h"

// WIESEP: This part should go into a cluster specific SDK

#define ITA_TILES(s, e, p) (s | (e << 4) | (p << 8))
#define ITA_LAYER(layer, activation) ((layer) | ((activation) << 2))
#define ITA_FLAGS(weight_preload, weight_nextload, bias_disable, bias_direction, output_disable) \
    ((weight_preload) | ((weight_nextload) << 1) | ((bias_disable) << 2) | \
     ((bias_direction) << 3) | ((output_disable) << 4))

static inline void __attribute((always_inline))
ita_set_addresses(uint32_t input_addr, uint32_t weight_addr, uint32_t weight_next_addr,
                  uint32_t bias_addr, uint32_t output_addr) {
    // Program ITA
    if (input_addr > CLUSTER_4_BASE) {
        *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x20) = input_addr - CLUSTER_4_BASE;
    }
    if (weight_addr > CLUSTER_4_BASE) {
        *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x24) = weight_addr - CLUSTER_4_BASE;
    }
    if (weight_next_addr > CLUSTER_4_BASE) {
        *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x28) =
            weight_next_addr - CLUSTER_4_BASE;
    }
    if (bias_addr > CLUSTER_4_BASE) {
        *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x2C) = bias_addr - CLUSTER_4_BASE;
    }
    if (output_addr > CLUSTER_4_BASE) {
        *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x30) = output_addr - CLUSTER_4_BASE;
    }
}

static inline void __attribute((always_inline)) ita_set_tiles(uint32_t tiles) {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x38) = tiles;
}

static inline void __attribute((always_inline)) ita_set_layers(uint32_t layer) {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x54) = layer; // ctrl engine
}

static inline void __attribute((always_inline)) ita_set_flags(uint32_t flags) {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x58) = flags; // ctrl stream
}

static inline void __attribute((always_inline)) ita_write_rqs_params(uint32_t eps1, uint32_t eps2,
                                                                     uint32_t right_shift1,
                                                                     uint32_t right_shift2,
                                                                     uint32_t add1, uint32_t add2) {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x3C) = eps1;
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x40) = eps2;
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x44) = right_shift1;
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x48) = right_shift2;
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x4C) = add1;
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x50) = add2;
}

static inline void __attribute((always_inline)) ita_soft_clear() {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x14) = 0;
    for (volatile int i = 0; i < 10; i++);
}

static inline void __attribute((always_inline)) ita_soft_clear_keep_regs() {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x14) = 1;
    for (volatile uint32_t i = 0; i < 10; i++);
}

static inline void __attribute((always_inline)) ita_acquire_job() {
    while (*(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x04) < 1);
}

static inline void __attribute((always_inline)) ita_wait_job() {
    // Do some dummy operaitons to avoid busy waiting too tightly
    // for (volatile int i = 0; i < 10; i++);
    while (*(volatile uint32_t *)(CLUSTER_4_HWPE_BUSY_ADDR) != 0);
}

static inline void __attribute((always_inline)) ita_trigger() {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x00) = 0;
}

static inline void __attribute((always_inline)) ita_commit() {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x00) = 1;
}

#define REDUCTION_FACTOR 2
#define DMA

static void ita_compute_qk_projection_step(
    const int8_t *const input_a_l2, const int8_t *const input_b_l2,
    const ita_int24_t *const bias_l2, const int8_t *output_l2, const int8_t *input_a_l1[2],
    const int8_t *input_b_l1[2], const ita_int24_t *bias_l1, const int8_t *output_l1[2],
    const int8_t *prev_output_tile_l2, const int8_t *const prev_output_tile_l1,
    int prev_output_tile_size, const int8_t *const next_step_input_b_tile_l2,
    const int8_t *next_step_input_b_tile_l1, int next_step_input_b_tile_size, int is_first_job) {
    int index_buff = 0;

    if (is_first_job) {
        // Load first tile of input_b
        const int tile_size_input_b = TILE_SIZE_PROJECTION_SPACE * TILE_SIZE_EMBEDDING_SPACE;
#ifdef DMA
        snrt_dma_start_1d((void *)input_b_l1[index_buff], (void *)input_b_l2,
                          tile_size_input_b / REDUCTION_FACTOR);
#endif
    }

    for (int i = 0; i < N_TILE_SEQUENCE_LENGTH; i++) {
        for (int j = 0; j < N_TILE_PROJECTION_SPACE; j++) {
            const int offset_bias = j * TILE_SIZE_PROJECTION_SPACE;
#ifdef DMA
            snrt_dma_start_1d((void *)bias_l1, (void *)(bias_l2 + offset_bias),
                              TILE_SIZE_PROJECTION_SPACE * sizeof(ita_int24_t) / REDUCTION_FACTOR);
#endif

            for (int k = 0; k < N_TILE_EMBEDDING_SPACE; k++) {
                const int is_first_tile = (i == 0) && (j == 0) && (k == 0);
                const int is_last_tile = (i == N_TILE_SEQUENCE_LENGTH - 1) &&
                                         (j == N_TILE_PROJECTION_SPACE - 1) &&
                                         (k == N_TILE_EMBEDDING_SPACE - 1);

                // Fetch inputs
                const int tile_size_input_a = TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_EMBEDDING_SPACE;
                const int offset_input_a = (i * N_TILE_EMBEDDING_SPACE + k) * tile_size_input_a;
#ifdef DMA
                snrt_dma_start_1d((void *)input_a_l1[index_buff],
                                  (void *)(input_a_l2 + offset_input_a),
                                  tile_size_input_a / REDUCTION_FACTOR);
#endif

                const int index_buff_output = (i * N_TILE_PROJECTION_SPACE + j) % 2;

                ita_set_addresses((uint32_t)input_a_l1[index_buff],
                                  (uint32_t)input_b_l1[index_buff],
                                  (uint32_t)(is_last_tile ? (uint32_t)next_step_input_b_tile_l1
                                                          : (uint32_t)input_b_l1[!index_buff]),
                                  (uint32_t)bias_l1, (uint32_t)output_l1[index_buff_output]);
                ita_set_flags((uint32_t)ITA_FLAGS(is_first_job && is_first_tile, 1, 0, 0,
                                                  k == N_TILE_EMBEDDING_SPACE - 1 ? 0 : 1));

                snrt_dma_wait_all();

                if (!(is_first_job && is_first_tile)) {
                    ita_wait_job();
                }

                if (is_last_tile) {
#ifdef DMA
                    snrt_dma_start_1d((void *)next_step_input_b_tile_l1,
                                      (void *)next_step_input_b_tile_l2,
                                      next_step_input_b_tile_size / REDUCTION_FACTOR);
#endif
                } else {
                    // Load next input_b tile
                    const int next_k = k == N_TILE_EMBEDDING_SPACE - 1 ? 0 : k + 1;
                    const int inc_j = j == N_TILE_PROJECTION_SPACE - 1 ? 0 : j + 1;
                    const int next_j = k == N_TILE_EMBEDDING_SPACE - 1 ? inc_j : j;
                    const int tile_size_input_b =
                        TILE_SIZE_PROJECTION_SPACE * TILE_SIZE_EMBEDDING_SPACE;
                    const int offset_input_b =
                        (next_j * N_TILE_EMBEDDING_SPACE + next_k) * tile_size_input_b;
#ifdef DMA
                    snrt_dma_start_1d((void *)input_b_l1[!index_buff],
                                      (void *)(input_b_l2 + offset_input_b),
                                      tile_size_input_b / REDUCTION_FACTOR);
#endif
                }

                ita_trigger();

                // Previous output tile writeback
                if (k == 0) {
                    if (i == 0 && j == 0) {
                        // Previous step's last output tile writeback
                        if (prev_output_tile_l2 != 0 && prev_output_tile_l1 != 0 &&
                            prev_output_tile_size != 0) {
#ifdef DMA
                            snrt_dma_start_1d((void *)prev_output_tile_l2,
                                              (void *)prev_output_tile_l1,
                                              prev_output_tile_size / REDUCTION_FACTOR);
#endif
                        }
                    } else {
                        const int prev_i = j == 0 ? i - 1 : i;
                        const int prev_j = j == 0 ? N_TILE_PROJECTION_SPACE - 1 : j - 1;
                        const int tile_size_output =
                            TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_PROJECTION_SPACE;
                        const int offset_output =
                            (prev_i * N_TILE_PROJECTION_SPACE + prev_j) * tile_size_output;
                        const int index_buff_output =
                            (prev_i * N_TILE_PROJECTION_SPACE + prev_j) % 2;
#ifdef DMA
                        snrt_dma_start_1d((void *)(output_l2 + offset_output),
                                          (void *)output_l1[index_buff_output],
                                          tile_size_output / REDUCTION_FACTOR);
#endif
                    }
                }

                index_buff = !index_buff;
            }
        }
    }
}

static void ita_compute_value_projection_step(
    const int8_t *const input_a_l2, const int8_t *const input_b_l2,
    const ita_int24_t *const bias_l2, const int8_t *output_l2, const int8_t *input_a_l1[2],
    const int8_t *input_b_l1[2], const ita_int24_t *bias_l1, const int8_t *output_l1[2],
    const int8_t *prev_output_tile_l2, const int8_t *const prev_output_tile_l1,
    int prev_output_tile_size, const int8_t *const next_step_input_b_tile_l2,
    const int8_t *next_step_input_b_tile_l1, int next_step_input_b_tile_size) {
    int index_buff = 0;

    for (int i = 0; i < N_TILE_PROJECTION_SPACE; i++) {
        const int offset_bias = i * TILE_SIZE_PROJECTION_SPACE;
#ifdef DMA
        snrt_dma_start_1d((void *)bias_l1, (void *)(bias_l2 + offset_bias),
                          TILE_SIZE_PROJECTION_SPACE * sizeof(ita_int24_t) / REDUCTION_FACTOR);
#endif

        for (int j = 0; j < N_TILE_SEQUENCE_LENGTH; j++) {
            for (int k = 0; k < N_TILE_EMBEDDING_SPACE; k++) {
                const int is_first_tile = (i == 0) && (j == 0) && (k == 0);
                const int is_last_tile = (i == N_TILE_PROJECTION_SPACE - 1) &&
                                         (j == N_TILE_SEQUENCE_LENGTH - 1) &&
                                         (k == N_TILE_EMBEDDING_SPACE - 1);

                // Fetch inputs
                const int tile_size_input_b =
                    TILE_SIZE_PROJECTION_SPACE * TILE_SIZE_EMBEDDING_SPACE;
                const int offset_input_b = (i * N_TILE_EMBEDDING_SPACE + k) * tile_size_input_b;
#ifdef DMA
                snrt_dma_start_1d((void *)input_b_l1[index_buff],
                                  (void *)(input_b_l2 + offset_input_b),
                                  tile_size_input_b / REDUCTION_FACTOR);
#endif

                const int index_buff_output = (i * N_TILE_SEQUENCE_LENGTH + j) % 2;

                ita_set_addresses((uint32_t)input_b_l1[index_buff],
                                  (uint32_t)input_a_l1[index_buff],
                                  (uint32_t)is_last_tile ? (uint32_t)next_step_input_b_tile_l1
                                                         : (uint32_t)input_a_l1[!index_buff],
                                  (uint32_t)bias_l1, (uint32_t)output_l1[index_buff_output]);
                ita_set_flags(
                    (uint32_t)ITA_FLAGS(0, 1, 0, 1, k == N_TILE_EMBEDDING_SPACE - 1 ? 0 : 1));

                snrt_dma_wait_all();

                ita_wait_job();

                if (is_last_tile) {
                    if (N_TILE_SEQUENCE_LENGTH == 1 && N_TILE_PROJECTION_SPACE == 1) {
                        // Only one Pk tile case
#ifdef DMA
                        snrt_dma_start_1d((void *)next_step_input_b_tile_l1,
                                          (void *)prev_output_tile_l1,
                                          next_step_input_b_tile_size / REDUCTION_FACTOR);
#endif
                        snrt_dma_wait_all();
                    } else {
#ifdef DMA
                        snrt_dma_start_1d((void *)next_step_input_b_tile_l1,
                                          (void *)next_step_input_b_tile_l2,
                                          next_step_input_b_tile_size / REDUCTION_FACTOR);
#endif
                    }
                } else {
                    const int next_k = k == N_TILE_EMBEDDING_SPACE - 1 ? 0 : k + 1;
                    const int inc_j = j == N_TILE_SEQUENCE_LENGTH - 1 ? 0 : j + 1;
                    const int next_j = k == N_TILE_EMBEDDING_SPACE - 1 ? inc_j : j;
                    const int tile_size_input_a =
                        TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_EMBEDDING_SPACE;
                    const int offset_input_a =
                        (next_j * N_TILE_EMBEDDING_SPACE + next_k) * tile_size_input_a;
#ifdef DMA
                    snrt_dma_start_1d((void *)input_a_l1[!index_buff],
                                      (void *)(input_a_l2 + offset_input_a),
                                      tile_size_input_a / REDUCTION_FACTOR);
#endif
                }

                ita_trigger();

                // Previous output tile writeback
                if (k == 0) {
                    if (i == 0 && j == 0) {
                        // Previous step's last output tile writeback
                        if (prev_output_tile_l2 != 0 && prev_output_tile_l1 != 0 &&
                            prev_output_tile_size != 0) {
#ifdef DMA
                            snrt_dma_start_1d((void *)prev_output_tile_l2,
                                              (void *)prev_output_tile_l1,
                                              prev_output_tile_size / REDUCTION_FACTOR);
#endif
                        }
                    } else {
                        const int prev_i = j == 0 ? i - 1 : i;
                        const int prev_j = j == 0 ? N_TILE_SEQUENCE_LENGTH - 1 : j - 1;
                        const int tile_size_output =
                            TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_PROJECTION_SPACE;
                        const int offset_output =
                            (prev_i * N_TILE_SEQUENCE_LENGTH + prev_j) * tile_size_output;
                        const int index_buff_output =
                            (prev_i * N_TILE_SEQUENCE_LENGTH + prev_j) % 2;
#ifdef DMA
                        snrt_dma_start_1d((void *)(output_l2 + offset_output),
                                          (void *)output_l1[index_buff_output],
                                          tile_size_output / REDUCTION_FACTOR);
#endif
                    }
                }

                index_buff = !index_buff;
            }
        }
    }
}

static void ita_compute_qk_row(const int8_t *const input_Pq_row_l2,
                               const int8_t *const input_Pk_row_l2, const int8_t *output_row_l2,
                               const int8_t *input_Pq_l1[2], const int8_t *input_Pk_l1[2],
                               const int8_t *output_l1[2], const int8_t *prev_output_tile_l2,
                               const int8_t *const prev_output_tile_l1, int prev_output_tile_size,
                               const int8_t *const next_step_input_b_tile_l2,
                               const int8_t *next_step_input_b_tile_l1,
                               int next_step_input_b_tile_size) {
    int index_buff = 0;

    for (int j = 0; j < N_TILE_SEQUENCE_LENGTH; j++) {
        for (int k = 0; k < N_TILE_PROJECTION_SPACE; k++) {
            const int is_first_tile = (j == 0) && (k == 0);
            const int is_last_tile =
                (j == N_TILE_SEQUENCE_LENGTH - 1) && (k == N_TILE_PROJECTION_SPACE - 1);

            // Fetch inputs
            const int tile_size_input_Pq = TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_PROJECTION_SPACE;
            const int offset_input_Pq = k * tile_size_input_Pq;
#ifdef DMA
            snrt_dma_start_1d((void *)input_Pq_l1[index_buff],
                              (void *)(input_Pq_row_l2 + offset_input_Pq),
                              tile_size_input_Pq / REDUCTION_FACTOR);
#endif

            const int index_buff_output = j % 2;

            ita_set_addresses((uint32_t)input_Pq_l1[index_buff], (uint32_t)input_Pk_l1[index_buff],
                              (uint32_t)is_last_tile ? (uint32_t)next_step_input_b_tile_l1
                                                     : (uint32_t)input_Pk_l1[!index_buff],
                              (uint32_t)0, (uint32_t)output_l1[index_buff_output]);
            ita_set_flags(
                (uint32_t)ITA_FLAGS(0, 1, 1, 0, k == N_TILE_PROJECTION_SPACE - 1 ? 0 : 1));

            snrt_dma_wait_all();

            ita_wait_job();

            if (is_last_tile) {
                if ((N_TILE_SEQUENCE_LENGTH == 1) && (N_TILE_PROJECTION_SPACE == 1)) {
                    // Only one Pv tile case
#ifdef DMA
                    snrt_dma_start_1d((void *)next_step_input_b_tile_l1,
                                      (void *)prev_output_tile_l1,
                                      next_step_input_b_tile_size / REDUCTION_FACTOR);
#endif
                    snrt_dma_wait_all();
                } else {
#ifdef DMA
                    snrt_dma_start_1d((void *)next_step_input_b_tile_l1,
                                      (void *)next_step_input_b_tile_l2,
                                      next_step_input_b_tile_size / REDUCTION_FACTOR);
#endif
                }
            } else {
                const int next_k = k == N_TILE_PROJECTION_SPACE - 1 ? 0 : k + 1;
                const int inc_j = j == N_TILE_SEQUENCE_LENGTH - 1 ? 0 : j + 1;
                const int next_j = k == N_TILE_PROJECTION_SPACE - 1 ? inc_j : j;
                const int tile_size_input_Pk =
                    TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_PROJECTION_SPACE;
                const int offset_input_Pk =
                    (next_j * N_TILE_PROJECTION_SPACE + next_k) * tile_size_input_Pk;
#ifdef DMA
                snrt_dma_start_1d((void *)input_Pk_l1[!index_buff],
                                  (void *)(input_Pk_row_l2 + offset_input_Pk),
                                  tile_size_input_Pk / REDUCTION_FACTOR);
#endif
            }

            ita_trigger();

            // Previous output tile writeback
            if (k == 0) {
                if (j == 0) {
#ifdef DMA
                    snrt_dma_start_1d((void *)prev_output_tile_l2, (void *)prev_output_tile_l1,
                                      prev_output_tile_size / REDUCTION_FACTOR);
#endif
                } else {
                    const int tile_size_output =
                        TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_SEQUENCE_LENGTH;
                    const int offset_output = (j - 1) * tile_size_output;
                    const int index_buff_output = (j - 1) % 2;
#ifdef DMA
                    snrt_dma_start_1d((void *)(output_row_l2 + offset_output),
                                      (void *)output_l1[index_buff_output],
                                      tile_size_output / REDUCTION_FACTOR);
#endif
                }
            }

            index_buff = !index_buff;
        }
    }
}

static void ita_compute_attention_row(
    const int8_t *const input_qk_row_l2, const int8_t *const input_Pv_row_l2,
    const int8_t *output_row_l2, const int8_t *input_qk_l1[2], const int8_t *input_Pv_l1[2],
    const int8_t *output_l1[2], const int8_t *prev_output_tile_l2,
    const int8_t *const prev_output_tile_l1, int prev_output_tile_size,
    const int8_t *const next_step_input_b_tile_l2, const int8_t *next_step_input_b_tile_l1,
    int next_step_input_b_tile_size) {
    int index_buff = 0;

    for (int j = 0; j < N_TILE_PROJECTION_SPACE; j++) {
        for (int k = 0; k < N_TILE_SEQUENCE_LENGTH; k++) {
            const int is_first_tile = (j == 0) && (k == 0);
            const int is_last_tile =
                (j == N_TILE_PROJECTION_SPACE - 1) && (k == N_TILE_SEQUENCE_LENGTH - 1);

            // Fetch inputs
            if (N_TILE_SEQUENCE_LENGTH > 1) {
                const int tile_size_input_qk =
                    TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_SEQUENCE_LENGTH;
                const int offset_input_qk = k * tile_size_input_qk;
#ifdef DMA
                snrt_dma_start_1d((void *)input_qk_l1[index_buff],
                                  (void *)(input_qk_row_l2 + offset_input_qk),
                                  tile_size_input_qk / REDUCTION_FACTOR);
#endif
            } else {
                input_qk_l1[index_buff] = prev_output_tile_l1;
            }

            const int index_buff_output = j % 2;

            ita_set_addresses((uint32_t)input_qk_l1[index_buff], (uint32_t)input_Pv_l1[index_buff],
                              (uint32_t)is_last_tile ? (uint32_t)next_step_input_b_tile_l1
                                                     : (uint32_t)input_Pv_l1[!index_buff],
                              (uint32_t)0, (uint32_t)output_l1[index_buff_output]);
            ita_set_flags((uint32_t)ITA_FLAGS(0, 1, 1, 0, k == N_TILE_SEQUENCE_LENGTH - 1 ? 0 : 1));

            snrt_dma_wait_all();

            ita_wait_job();

            if (is_last_tile) {
#ifdef DMA
                snrt_dma_start_1d((void *)next_step_input_b_tile_l1,
                                  (void *)next_step_input_b_tile_l2,
                                  next_step_input_b_tile_size / REDUCTION_FACTOR);
#endif
            } else {
                const int next_k = k == N_TILE_SEQUENCE_LENGTH - 1 ? 0 : k + 1;
                const int inc_j = j == N_TILE_PROJECTION_SPACE - 1 ? 0 : j + 1;
                const int next_j = k == N_TILE_SEQUENCE_LENGTH - 1 ? inc_j : j;
                const int tile_size_input_Pv =
                    TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_PROJECTION_SPACE;
                const int offset_input_Pv =
                    (next_j * N_TILE_SEQUENCE_LENGTH + next_k) * tile_size_input_Pv;
#ifdef DMA
                snrt_dma_start_1d((void *)input_Pv_l1[!index_buff],
                                  (void *)(input_Pv_row_l2 + offset_input_Pv),
                                  tile_size_input_Pv / REDUCTION_FACTOR);
#endif
            }

            ita_trigger();

            // Previous output tile writeback
            if (k == 0) {
                if (j == 0) {
#ifdef DMA
                    snrt_dma_start_1d((void *)prev_output_tile_l2, (void *)prev_output_tile_l1,
                                      prev_output_tile_size / REDUCTION_FACTOR);
#endif
                    snrt_dma_wait_all();
                } else {
                    const int tile_size_output =
                        TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_PROJECTION_SPACE;
                    const int offset_output = (j - 1) * tile_size_output;
                    const int index_buff_output = (j - 1) % 2;
#ifdef DMA
                    snrt_dma_start_1d((void *)(output_row_l2 + offset_output),
                                      (void *)output_l1[index_buff_output],
                                      tile_size_output / REDUCTION_FACTOR);
#endif
                }
            }

            index_buff = !index_buff;
        }
    }
}

static void
ita_compute_output_embedding_step(const int8_t *const input_av_l2, const int8_t *const input_Wo_l2,
                                  const ita_int24_t *const bias_l2, const int8_t *output_l2,
                                  const int8_t *input_av_l1[2], const int8_t *input_Wo_l1[2],
                                  const ita_int24_t *bias_l1, const int8_t *output_l1[2],
                                  const int8_t *const prev_output_tile_l2,
                                  const int8_t *prev_output_tile_l1, int prev_output_tile_size) {
    int index_buff = 0;

    for (int i = 0; i < N_TILE_SEQUENCE_LENGTH; i++) {
        for (int j = 0; j < N_TILE_EMBEDDING_SPACE; j++) {
            const int offset_bias = j * TILE_SIZE_EMBEDDING_SPACE;
#ifdef DMA
            snrt_dma_start_1d((void *)bias_l1, (void *)(bias_l2 + offset_bias),
                              TILE_SIZE_EMBEDDING_SPACE * sizeof(ita_int24_t) / REDUCTION_FACTOR);
#endif

            for (int k = 0; k < N_TILE_PROJECTION_SPACE; k++) {
                const int is_first_tile = (i == 0) && (j == 0) && (k == 0);
                const int is_last_tile = (i == N_TILE_SEQUENCE_LENGTH - 1) &&
                                         (j == N_TILE_EMBEDDING_SPACE - 1) &&
                                         (k == N_TILE_PROJECTION_SPACE - 1);

                if ((N_TILE_SEQUENCE_LENGTH > 1) && (N_TILE_EMBEDDING_SPACE > 1)) {
                    // Fetch inputs
                    const int offset_input_av =
                        (i * N_TILE_PROJECTION_SPACE + k) * prev_output_tile_size;
#ifdef DMA
                    snrt_dma_start_1d((void *)input_av_l1[index_buff],
                                      (void *)(input_av_l2 + offset_input_av),
                                      prev_output_tile_size / REDUCTION_FACTOR);
#endif
                } else {
                    // Only one tile case
                    input_av_l1[index_buff] = prev_output_tile_l1;
                }

                const int index_buff_output = (i * N_TILE_EMBEDDING_SPACE + j) % 2;

                ita_set_addresses((uint32_t)input_av_l1[index_buff],
                                  (uint32_t)input_Wo_l1[index_buff],
                                  (uint32_t)input_Wo_l1[!index_buff], (uint32_t)bias_l1,
                                  (uint32_t)output_l1[index_buff_output]);
                ita_set_flags((uint32_t)ITA_FLAGS(0, !is_last_tile, 0, 0,
                                                  k == N_TILE_PROJECTION_SPACE - 1 ? 0 : 1));

                snrt_dma_wait_all();

                ita_wait_job();

                if (!is_last_tile) {
                    const int next_k = k == N_TILE_PROJECTION_SPACE - 1 ? 0 : k + 1;
                    const int inc_j = j == N_TILE_EMBEDDING_SPACE - 1 ? 0 : j + 1;
                    const int next_j = k == N_TILE_PROJECTION_SPACE - 1 ? inc_j : j;
                    const int tile_size_input_Wo =
                        TILE_SIZE_EMBEDDING_SPACE * TILE_SIZE_PROJECTION_SPACE;
                    const int offset_input_Wo =
                        (next_j * N_TILE_PROJECTION_SPACE + next_k) * tile_size_input_Wo;
#ifdef DMA
                    snrt_dma_start_1d((void *)input_Wo_l1[!index_buff],
                                      (void *)(input_Wo_l2 + offset_input_Wo),
                                      tile_size_input_Wo / REDUCTION_FACTOR);
#endif
                }

                ita_trigger();

                // Previous output tile writeback
                if (k == 0) {
                    if (i == 0 && j == 0) {
                        // Previous step's last output tile writeback
                        if (prev_output_tile_l2 != 0 && prev_output_tile_l1 != 0 &&
                            prev_output_tile_size != 0) {
#ifdef DMA
                            snrt_dma_start_1d((void *)prev_output_tile_l2,
                                              (void *)prev_output_tile_l1,
                                              prev_output_tile_size / REDUCTION_FACTOR);
#endif
                        }
                    } else {
                        const int prev_i = j == 0 ? i - 1 : i;
                        const int prev_j = j == 0 ? N_TILE_EMBEDDING_SPACE - 1 : j - 1;
                        const int tile_size_output =
                            TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_EMBEDDING_SPACE;
                        const int offset_output =
                            (prev_i * N_TILE_EMBEDDING_SPACE + prev_j) * tile_size_output;
                        const int index_buff_output =
                            (prev_i * N_TILE_EMBEDDING_SPACE + prev_j) % 2;
#ifdef DMA
                        snrt_dma_start_1d((void *)(output_l2 + offset_output),
                                          (void *)output_l1[index_buff_output],
                                          tile_size_output / REDUCTION_FACTOR);
#endif
                    }
                }

                index_buff = !index_buff;
            }
        }
    }
}

static inline void __attribute((always_inline)) ita_compute_matmul_L1(const int8_t *input_a_l1,
                                                                      const int8_t *input_b_l1,
                                                                      const ita_int24_t *bias_l1,
                                                                      const int8_t *output_l1) {
    for (int i = 0; i < N_TILE_SEQUENCE_LENGTH; i++) {
        for (int j = 0; j < N_TILE_PROJECTION_SPACE; j++) {
            const int offset_bias = j * TILE_SIZE_PROJECTION_SPACE;

            for (int k = 0; k < N_TILE_EMBEDDING_SPACE; k++) {
                const int is_first_tile = (i == 0) && (j == 0) && (k == 0);
                const int is_last_tile = (i == N_TILE_SEQUENCE_LENGTH - 1) &&
                                         (j == N_TILE_PROJECTION_SPACE - 1) &&
                                         (k == N_TILE_EMBEDDING_SPACE - 1);

                const int tile_size_input_a = TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_EMBEDDING_SPACE;
                const int offset_input_a = (i * N_TILE_EMBEDDING_SPACE + k) * tile_size_input_a;

                const int tile_size_input_b =
                    TILE_SIZE_PROJECTION_SPACE * TILE_SIZE_EMBEDDING_SPACE;
                const int offset_input_b = (j * N_TILE_EMBEDDING_SPACE + k) * tile_size_input_b;

                const int offest_input_b_next =
                    (j * N_TILE_EMBEDDING_SPACE + ((k + 1) % N_TILE_EMBEDDING_SPACE)) *
                    tile_size_input_b;

                const int tile_size_output = TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_PROJECTION_SPACE;
                const int offset_output = (i * N_TILE_PROJECTION_SPACE + j) * tile_size_output;

                ita_set_addresses((uint32_t)(input_a_l1 + offset_input_a),
                                  (uint32_t)(input_b_l1 + offset_input_b),
                                  (uint32_t)(input_b_l1 + offest_input_b_next),
                                  (uint32_t)(bias_l1 + offset_bias),
                                  (uint32_t)(output_l1 + offset_output));
                ita_set_flags((uint32_t)ITA_FLAGS(is_first_tile, !is_last_tile, 0, 0,
                                                  k == N_TILE_EMBEDDING_SPACE - 1 ? 0 : 1));

                snrt_dma_wait_all();

                if (!is_first_tile) {
                    ita_wait_job();
                }

                ita_trigger();
            }
        }
    }
}

static inline void __attribute((always_inline))
ita_compute_matmul_L2(const int8_t *const input_a_l2, const int8_t *const input_b_l2,
                      const ita_int24_t *const bias_l2, const int8_t *output_l2,
                      const int8_t *input_a_l1[2], const int8_t *input_b_l1[2],
                      const ita_int24_t *bias_l1, const int8_t *output_l1[2]) {
    int index_buff = 0;

    // Load first tile of input_b
    const int tile_size_input_b = TILE_SIZE_PROJECTION_SPACE * TILE_SIZE_EMBEDDING_SPACE;
    snrt_dma_start_1d((void *)input_b_l1[index_buff], (void *)input_b_l2, tile_size_input_b);

    for (int i = 0; i < N_TILE_SEQUENCE_LENGTH; i++) {
        for (int j = 0; j < N_TILE_PROJECTION_SPACE; j++) {
            const int offset_bias = j * TILE_SIZE_PROJECTION_SPACE;
            snrt_dma_start_1d((void *)bias_l1, (void *)(bias_l2 + offset_bias),
                              TILE_SIZE_PROJECTION_SPACE * sizeof(ita_int24_t));

            for (int k = 0; k < N_TILE_EMBEDDING_SPACE; k++) {
                const int is_first_tile = (i == 0) && (j == 0) && (k == 0);
                const int is_last_tile = (i == N_TILE_SEQUENCE_LENGTH - 1) &&
                                         (j == N_TILE_PROJECTION_SPACE - 1) &&
                                         (k == N_TILE_EMBEDDING_SPACE - 1);

                // Fetch inputs
                const int tile_size_input_a = TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_EMBEDDING_SPACE;
                const int offset_input_a = (i * N_TILE_EMBEDDING_SPACE + k) * tile_size_input_a;
                snrt_dma_start_1d((void *)input_a_l1[index_buff],
                                  (void *)(input_a_l2 + offset_input_a), tile_size_input_a);

                const int index_buff_output = (i * N_TILE_PROJECTION_SPACE + j) % 2;

                ita_set_addresses((uint32_t)input_a_l1[index_buff],
                                  (uint32_t)input_b_l1[index_buff],
                                  (uint32_t)input_b_l1[!index_buff], (uint32_t)bias_l1,
                                  (uint32_t)output_l1[index_buff_output]);
                ita_set_flags((uint32_t)ITA_FLAGS(is_first_tile, !is_last_tile, 0, 0,
                                                  k == N_TILE_EMBEDDING_SPACE - 1 ? 0 : 1));

                snrt_dma_wait_all();

                if (!is_first_tile) {
                    ita_wait_job();
                }

                if (!is_last_tile) {
                    // Load next input_b tile
                    const int next_k = k == N_TILE_EMBEDDING_SPACE - 1 ? 0 : k + 1;
                    const int inc_j = j == N_TILE_PROJECTION_SPACE - 1 ? 0 : j + 1;
                    const int next_j = k == N_TILE_EMBEDDING_SPACE - 1 ? inc_j : j;
                    const int tile_size_input_b =
                        TILE_SIZE_PROJECTION_SPACE * TILE_SIZE_EMBEDDING_SPACE;
                    const int offset_input_b =
                        (next_j * N_TILE_EMBEDDING_SPACE + next_k) * tile_size_input_b;
                    snrt_dma_start_1d((void *)input_b_l1[!index_buff],
                                      (void *)(input_b_l2 + offset_input_b), tile_size_input_b);
                }

                ita_trigger();

                // Previous output tile writeback
                if (k == 0) {
                    if (i != 0 || j != 0) {
                        const int prev_i = j == 0 ? i - 1 : i;
                        const int prev_j = j == 0 ? N_TILE_PROJECTION_SPACE - 1 : j - 1;
                        const int tile_size_output =
                            TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_PROJECTION_SPACE;
                        const int offset_output =
                            (prev_i * N_TILE_PROJECTION_SPACE + prev_j) * tile_size_output;
                        const int index_buff_output =
                            (prev_i * N_TILE_PROJECTION_SPACE + prev_j) % 2;
                        snrt_dma_start_1d((void *)(output_l2 + offset_output),
                                          (void *)output_l1[index_buff_output], tile_size_output);
                    }
                }

                index_buff = !index_buff;
            }
        }
    }
}

#endif //_ITA_CTRL_INCLUDE_GUARD_
