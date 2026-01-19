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

// Import HAL Headers

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

// #define DEBUG

/**
 * @brief Main function of the cluster test.
 *
 * @return int Return 0 if the test was successful, -1 otherwise.
 */
int32_t ita_sha_l2_test(void *args) {
    int32_t tot_err = 0;

    test_cluster_args_t *test_args = (test_cluster_args_t *)args;
    test_cluster_result_t *test_retVal = (test_cluster_result_t *)(test_args->result);
    ita_sha_l2_test_args_t *user_args = (ita_sha_l2_test_args_t *)(test_args->args);

    /*
     * Initialize the Snitch runtime.
     */
    snrt_init();

    /********** Cluster Initialization **********/
    if (snrt_is_dm_core()) {
        printf("Starting ITA SHA execution (S=%u, E=%u P=%u) for %d iterations\n", SEQUENCE_LENGTH,
               EMBEDDING_SPACE, PROJECTION_SPACE, test_args->repetitions);

        // printf("Core %d: Setting up ITA Test...\n", snrt_cluster_core_idx());
        const int8_t *interm_Pq = user_args->interm_Pq;
        const int8_t *interm_Pk = user_args->interm_Pk;
        const int8_t *interm_Pv = user_args->interm_Pv;
        const int8_t *interm_qk = user_args->interm_qk;
        const int8_t *interm_attention = user_args->interm_attention;
        const int8_t *interm_output = user_args->interm_output;

        // L1 buffers
        const int8_t *input_a0_buff[2];
        input_a0_buff[0] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        input_a0_buff[1] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        const int8_t *input_a1_buff[2];
        input_a1_buff[0] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        input_a1_buff[1] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        const int8_t *input_b0_buff[2];
        input_b0_buff[0] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        input_b0_buff[1] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        const int8_t *input_b1_buff[2];
        input_b1_buff[0] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        input_b1_buff[1] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        const int8_t *input_b2_buff[2];
        input_b2_buff[0] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        input_b2_buff[1] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        const int8_t *output_0_buff[2];
        output_0_buff[0] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        output_0_buff[1] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        const int8_t *output_1_buff[2];
        output_1_buff[0] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        output_1_buff[1] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        const int8_t *output_2_buff[2];
        output_2_buff[0] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        output_2_buff[1] = snrt_l1_alloc(MAX_BUFFER_SIZE);
        const ita_int24_t *input_bias_0 = snrt_l1_alloc(MAX_BUFFER_BIAS_SIZE);
        const ita_int24_t *input_bias_1 = snrt_l1_alloc(MAX_BUFFER_BIAS_SIZE);

        const int8_t *l1_arena_start = input_a0_buff[0];
        const int8_t *l1_arena_end = (int8_t *)snrt_l1_allocator()->next;
        const size_t l1_arena_size = l1_arena_end - l1_arena_start;

        // Initialize all buffers with values from L2
        uint32_t reps = l1_arena_size / (SEQUENCE_LENGTH * EMBEDDING_SPACE);
        const size_t input_q_size = SEQUENCE_LENGTH * EMBEDDING_SPACE;
        // printf("Initializing L1 arena of size %u bytes with %u repetitions of input_q (%u bytes
        // each)\n",
        //        (unsigned int)l1_arena_size, (unsigned int)reps, (unsigned int)input_q_size);
        for (uint32_t i = 0; i < reps; i++) {
            snrt_dma_start_1d((void *)l1_arena_start + i * input_q_size, (void *)input_q,
                              input_q_size);
        }
        // Transfer remeining bytes if any
        size_t remaining_bytes = l1_arena_size % input_q_size;
        if (remaining_bytes > 0) {
            // printf("Transferring remaining %u bytes to L1 arena\n", (unsigned
            // int)remaining_bytes);
            snrt_dma_start_1d((void *)l1_arena_start + reps * input_q_size, (void *)input_q,
                              remaining_bytes);
        }
        snrt_dma_wait_all();

        // Initialize golden results pointer
        // snrt_dma_start_1d((void *)interm_Pq, (void *)golden_interm_Pq,
        //                   SEQUENCE_LENGTH * PROJECTION_SPACE);
        // snrt_dma_start_1d((void *)interm_Pk, (void *)golden_interm_Pk,
        //                   SEQUENCE_LENGTH * PROJECTION_SPACE);
        // snrt_dma_start_1d((void *)interm_Pv, (void *)golden_interm_Pv,
        //                   SEQUENCE_LENGTH * PROJECTION_SPACE);
        // snrt_dma_start_1d((void *)interm_qk, (void *)golden_interm_attention,
        //                   SEQUENCE_LENGTH * SEQUENCE_LENGTH);
        // snrt_dma_start_1d((void *)interm_attention, (void *)golden_interm_head_output,
        //                   SEQUENCE_LENGTH * SEQUENCE_LENGTH);
        // snrt_dma_start_1d((void *)interm_output, (void *)golden_output,
        //                   SEQUENCE_LENGTH * EMBEDDING_SPACE);
        // snrt_dma_wait_all();

        // Properly reset ITA
        ita_soft_clear();
        ita_acquire_job();

        // Initialize all ITA contexts
        // WIESEP: This is required as otherwise the ITA FSM does invalid state transitions, because
        // the HPWE context and thus register file is switched as soon as the current job is
        // finished.
        for (int i = 0; i < N_CONTEXT; i++) {
            ita_set_layers(ITA_LAYER(ATTENTION, IDENTITY));
            ita_set_tiles(
                ITA_TILES(N_TILE_SEQUENCE_LENGTH, N_TILE_EMBEDDING_SPACE, N_TILE_PROJECTION_SPACE));
            ita_write_rqs_params(requant_eps_mult[0][0], requant_eps_mult[0][1],
                                 requant_right_shift[0][0], requant_right_shift[0][1],
                                 requant_add[0][0], requant_add[0][1]);
            ita_commit();
        }
        printf("ITA contexts initialized.\n");

        uint32_t start_cycles = 0, end_cycles = 0;
        uint32_t start_instructions = 0, end_instructions = 0;

        start_cycles = snrt_mcycle();
        start_instructions = snrt_minstret();

        for (int i = 0; i < test_args->repetitions; i++) {
            // if (i == test_args->repetitions - 1) {
            //     // Clear output buffers in L2 before last iteration to avoid false positives
            //     snrt_dma_start_1d((void *)interm_output, (void *)snrt_zero_memory_ptr(),
            //                       SEQUENCE_LENGTH * EMBEDDING_SPACE);
            //     snrt_dma_wait_all();
            // }

            ita_soft_clear_keep_regs();
            ita_acquire_job();

            const int tile_size_Wk = TILE_SIZE_PROJECTION_SPACE * TILE_SIZE_EMBEDDING_SPACE;

#ifdef DEBUG
            printf("Starting ITA Step Q Projection...\n");
#endif

            ita_compute_qk_projection_step(input_q, input_Wq[0], input_Bq[0], interm_Pq,
                                           input_a0_buff, input_b0_buff, input_bias_0,
                                           output_0_buff, 0, 0, 0, input_Wk[0], input_b1_buff[0],
                                           tile_size_Wk, 1);

            const int prev_i_Pq = N_TILE_SEQUENCE_LENGTH - 1;
            const int prev_j_Pq = N_TILE_PROJECTION_SPACE - 1;
            const int tile_size_Pq = TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_PROJECTION_SPACE;
            const int offset_Pq = (prev_i_Pq * N_TILE_PROJECTION_SPACE + prev_j_Pq) * tile_size_Pq;
            const int8_t *interm_Pq_tile_ptr = interm_Pq + offset_Pq;
            const int index_buff_Pq = (N_TILE_SEQUENCE_LENGTH * N_TILE_PROJECTION_SPACE - 1) % 2;

            const int tile_size_k = TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_EMBEDDING_SPACE;

#ifdef DEBUG
            printf("Starting ITA Step K Projection...\n");
#endif
            ita_compute_qk_projection_step(
                input_k, input_Wk[0], input_Bk[0], interm_Pk, input_a1_buff, input_b1_buff,
                input_bias_1, output_1_buff, interm_Pq_tile_ptr, output_0_buff[index_buff_Pq],
                tile_size_Pq, input_k, input_b2_buff[0], tile_size_k, 0);

            const int prev_i_Pk = N_TILE_SEQUENCE_LENGTH - 1;
            const int prev_j_Pk = N_TILE_PROJECTION_SPACE - 1;
            const int tile_size_Pk = TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_PROJECTION_SPACE;
            const int offset_Pk = (prev_i_Pk * N_TILE_PROJECTION_SPACE + prev_j_Pk) * tile_size_Pk;
            const int8_t *interm_Pk_tile_ptr = interm_Pk + offset_Pk;
            const int index_buff_Pk = (N_TILE_SEQUENCE_LENGTH * N_TILE_PROJECTION_SPACE - 1) % 2;

#ifdef DEBUG
            printf("Starting ITA Step V Projection...\n");
#endif
            ita_compute_value_projection_step(
                input_k, input_Wv[0], input_Bv[0], interm_Pv, input_b2_buff, input_a0_buff,
                input_bias_0, output_2_buff, interm_Pk_tile_ptr, output_1_buff[index_buff_Pk],
                tile_size_Pk, interm_Pk, input_b0_buff[0], tile_size_Pk);

            const int prev_i_Pv = N_TILE_PROJECTION_SPACE - 1;
            const int prev_j_Pv = N_TILE_SEQUENCE_LENGTH - 1;
            const int tile_size_Pv = TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_PROJECTION_SPACE;
            const int offset_Pv = (prev_i_Pv * N_TILE_SEQUENCE_LENGTH + prev_j_Pv) * tile_size_Pv;
            const int8_t *interm_Pv_tile_ptr = interm_Pv + offset_Pv;
            const int index_buff_Pv = (N_TILE_SEQUENCE_LENGTH * N_TILE_PROJECTION_SPACE - 1) % 2;

            const int tile_size_Wo = TILE_SIZE_EMBEDDING_SPACE * TILE_SIZE_PROJECTION_SPACE;

#ifdef DEBUG
            printf("Starting ITA Steps QK and Softmax...\n");
#endif
            // Steps 4 & 5
            for (int i = 0; i < N_TILE_SEQUENCE_LENGTH; i++) {
                int8_t *prev_tile_ptr;
                int8_t *local_prev_tile_ptr;
                int prev_tile_size;

                if (i == 0) {
                    prev_tile_ptr = (int8_t *)interm_Pv_tile_ptr;
                    local_prev_tile_ptr = (int8_t *)output_2_buff[index_buff_Pv];
                    prev_tile_size = tile_size_Pv;
                } else {
                    const int tile_size_attention =
                        TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_PROJECTION_SPACE;
                    const int prev_i_attention = i - 1;
                    const int prev_j_attention = N_TILE_PROJECTION_SPACE - 1;
                    const int offset_attention =
                        (prev_i_attention * N_TILE_PROJECTION_SPACE + prev_j_attention) *
                        tile_size_attention;
                    const int8_t *interm_attention_tile_ptr = interm_attention + offset_attention;
                    const int index_buff_attention = (N_TILE_PROJECTION_SPACE - 1) % 2;

                    prev_tile_ptr = (int8_t *)interm_attention_tile_ptr;
                    local_prev_tile_ptr = (int8_t *)output_1_buff[index_buff_attention];
                    prev_tile_size = tile_size_attention;
                }

                const int8_t *interm_Pq_row = interm_Pq + i * N_TILE_PROJECTION_SPACE *
                                                              TILE_SIZE_SEQUENCE_LENGTH *
                                                              TILE_SIZE_PROJECTION_SPACE;
                const int8_t *interm_qk_row = interm_qk + i * N_TILE_SEQUENCE_LENGTH *
                                                              TILE_SIZE_SEQUENCE_LENGTH *
                                                              TILE_SIZE_SEQUENCE_LENGTH;

#ifdef DEBUG
                printf("Processing ITA Step QK Row %d/%d...\n", i + 1, N_TILE_SEQUENCE_LENGTH);
#endif
                ita_compute_qk_row(interm_Pq_row, interm_Pk, interm_qk_row, input_a1_buff,
                                   input_b0_buff, output_0_buff, prev_tile_ptr, local_prev_tile_ptr,
                                   prev_tile_size, interm_Pv, input_b1_buff[0], tile_size_Pv);

                const int tile_size_qk = TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_SEQUENCE_LENGTH;
                const int offset_qk = (N_TILE_SEQUENCE_LENGTH - 1) * tile_size_qk;
                const int8_t *interm_qk_tile_ptr = interm_qk_row + offset_qk;
                const int index_buff_qk = (N_TILE_SEQUENCE_LENGTH - 1) % 2;

                const int8_t *interm_attention_row =
                    interm_attention + i * N_TILE_PROJECTION_SPACE * TILE_SIZE_SEQUENCE_LENGTH *
                                           TILE_SIZE_PROJECTION_SPACE;
                const int is_last_row = i == N_TILE_SEQUENCE_LENGTH - 1;

#ifdef DEBUG
                printf("Processing ITA Step Attention Row %d/%d...\n", i + 1,
                       N_TILE_SEQUENCE_LENGTH);
#endif
                ita_compute_attention_row(interm_qk_row, interm_Pv, interm_attention_row,
                                          input_a0_buff, input_b1_buff, output_1_buff,
                                          interm_qk_tile_ptr, output_0_buff[index_buff_qk],
                                          tile_size_qk, is_last_row ? input_Wo[0] : interm_Pk,
                                          is_last_row ? input_b2_buff[0] : input_b0_buff[0],
                                          is_last_row ? tile_size_Wo : tile_size_Pk);
            }

            // Send back last tile
            const int prev_i_attention = N_TILE_SEQUENCE_LENGTH - 1;
            const int prev_j_attention = N_TILE_PROJECTION_SPACE - 1;
            const int tile_size_attention = TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_PROJECTION_SPACE;
            const int offset_attention =
                (prev_i_attention * N_TILE_PROJECTION_SPACE + prev_j_attention) *
                tile_size_attention;
            const int8_t *interm_attention_tile_ptr = interm_attention + offset_attention;
            const int index_buff_attention = (N_TILE_PROJECTION_SPACE - 1) % 2;

#ifdef DEBUG
            printf("Starting ITA Step Output Projection...\n");
#endif
            // Use q_buff_l1 just as an intermediate buffer to not overwrite attention_buff
            ita_compute_output_embedding_step(
                interm_attention, input_Wo[0], input_Bo[0], interm_output, input_a1_buff,
                input_b2_buff, input_bias_0, output_2_buff, interm_attention_tile_ptr,
                output_1_buff[index_buff_attention], tile_size_attention);

            ita_wait_job();

#ifdef DEBUG
            printf("Done with ITA computation, starting DMA transfers back to L3...\n");
#endif

            // Send back last tile
            const int prev_i_output = N_TILE_SEQUENCE_LENGTH - 1;
            const int prev_j_output = N_TILE_EMBEDDING_SPACE - 1;
            const int tile_size_output = TILE_SIZE_SEQUENCE_LENGTH * TILE_SIZE_EMBEDDING_SPACE;
            const int offset_output =
                (prev_i_output * N_TILE_EMBEDDING_SPACE + prev_j_output) * tile_size_output;
            const int8_t *interm_output_tile_ptr = interm_output + offset_output;
            const int index_buff_output = (N_TILE_SEQUENCE_LENGTH * N_TILE_EMBEDDING_SPACE - 1) % 2;
#ifdef DMA
            // Send back last tile
            snrt_dma_start_1d((void *)interm_output_tile_ptr,
                              (void *)output_2_buff[index_buff_output],
                              tile_size_output / REDUCTION_FACTOR);
#endif
            snrt_dma_wait_all();
        }
        end_cycles = snrt_mcycle();
        end_instructions = snrt_minstret();

        printf("ITA MatMul cycles = %u\n", end_cycles - start_cycles);
        printf("ITA MatMul instructions = %u\n", end_instructions - start_instructions);

        float ops = (float)SHA_OPS * test_args->repetitions; // MACs
        float ops_per_cycle = (float)ops / (float)(end_cycles - start_cycles);

#ifdef DEBUG
        printf("Starting ITA Checking...\n");
#endif
        // tot_err += check(1, (uint8_t *)interm_Pq, (uint8_t *)golden_interm_Pq,
        //                  SEQUENCE_LENGTH * PROJECTION_SPACE, l1_arena_start, 16 * 4096);
        // tot_err += check(2, (uint8_t *)interm_Pk, (uint8_t *)golden_interm_Pk,
        //                  SEQUENCE_LENGTH * PROJECTION_SPACE, l1_arena_start, 16 * 4096);
        // tot_err += check(3, (uint8_t *)interm_Pv, (uint8_t *)golden_interm_Pv,
        //                  SEQUENCE_LENGTH * PROJECTION_SPACE, l1_arena_start, 16 * 4096);
        // tot_err += check(4, (uint8_t *)interm_qk, (uint8_t *)golden_interm_attention,
        //                  SEQUENCE_LENGTH * SEQUENCE_LENGTH, l1_arena_start, 16 * 4096);
        // tot_err += check(5, (uint8_t *)interm_attention, (uint8_t *)golden_interm_head_output,
        //                  SEQUENCE_LENGTH * PROJECTION_SPACE, l1_arena_start, 16 * 4096);
        // tot_err += check(6, (uint8_t *)interm_output, (uint8_t *)golden_output,
        //                  SEQUENCE_LENGTH * EMBEDDING_SPACE, l1_arena_start, 16 * 4096);

        // if (tot_err != 0) {
        //     printf("Test failed with %d errors\r\n", tot_err);
        // }

        test_retVal->errors = tot_err;
        test_retVal->ops_per_cycle = (uint32_t)(ops_per_cycle * 1e6);
        test_retVal->runtime_cycles = end_cycles - start_cycles;
    }

    snrt_cluster_hw_barrier();

    return tot_err << 1;
}
