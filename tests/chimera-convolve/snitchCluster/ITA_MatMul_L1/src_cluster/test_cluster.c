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
int32_t ita_matmul_l1_test(void *args) {
    int32_t tot_err = 0;

    test_cluster_args_t *test_args = (test_cluster_args_t *)args;
    test_cluster_result_t *test_retVal = (test_cluster_result_t *)(test_args->result);

    /*
     * Initialize the Snitch runtime.
     */
    snrt_init();

    /********** Cluster Initialization **********/
    if (snrt_is_dm_core()) {
        printf("Starting ITA MatMul execution (%ux%u x %ux%u = %ux%u) for %d iterations\n",
               SEQUENCE_LENGTH, EMBEDDING_SPACE, EMBEDDING_SPACE, PROJECTION_SPACE, SEQUENCE_LENGTH,
               PROJECTION_SPACE, test_args->repetitions);

        // L1 buffers
        const int8_t *input_a0_buff = snrt_l1_alloc(SEQUENCE_LENGTH * EMBEDDING_SPACE);
        const int8_t *input_b0_buff = snrt_l1_alloc(EMBEDDING_SPACE * PROJECTION_SPACE);
        const ita_int24_t *input_bias_0 = snrt_l1_alloc(PROJECTION_SPACE * sizeof(ita_int24_t));
        const int8_t *output_0_buff = snrt_l1_alloc(SEQUENCE_LENGTH * PROJECTION_SPACE);
        const int8_t *l1_arena_start = input_a0_buff;
        const int8_t *l1_arena_end = (int8_t *)snrt_l1_allocator()->next;

        // Copy data from L2 to L1
        snrt_dma_start_1d((void *)input_a0_buff, (void *)input_q,
                          SEQUENCE_LENGTH * EMBEDDING_SPACE);
        snrt_dma_start_1d((void *)input_b0_buff, (void *)input_Wq,
                          EMBEDDING_SPACE * PROJECTION_SPACE);
        snrt_dma_start_1d((void *)input_bias_0, (void *)input_Bq,
                          PROJECTION_SPACE * sizeof(ita_int24_t));
        snrt_dma_wait_all();

        // Properly reset ITA
        ita_soft_clear();
        ita_acquire_job();

        // Initialize all ITA contexts
        // WIESEP: This is required as otherwise the ITA FSM does invalid state transitions, because
        // the HPWE context and thus register file is switched as soon as the current job is
        // finished.
        for (int i = 0; i < N_CONTEXT; i++) {
            ita_set_layers(ITA_LAYER(LINEAR, IDENTITY));
            ita_set_tiles(
                ITA_TILES(N_TILE_SEQUENCE_LENGTH, N_TILE_EMBEDDING_SPACE, N_TILE_PROJECTION_SPACE));
            ita_write_rqs_params(requant_eps_mult[0][0], requant_eps_mult[0][1],
                                 requant_right_shift[0][0], requant_right_shift[0][1],
                                 requant_add[0][0], requant_add[0][1]);
            ita_commit();
        }

        uint32_t start_cycles = 0, end_cycles = 0;
        uint32_t start_instructions = 0, end_instructions = 0;

        start_cycles = snrt_mcycle();
        start_instructions = snrt_minstret();

        for (int i = 0; i < test_args->repetitions; i++) {
            ita_soft_clear_keep_regs();
            ita_acquire_job();

            ita_compute_matmul_L1(input_a0_buff, // input_a0_buff
                                  input_b0_buff, // input_b0_buff
                                  input_bias_0,  // input_bias_0
                                  output_0_buff  // output_0_buff
            );

            ita_wait_job();
        }
        end_cycles = snrt_mcycle();
        end_instructions = snrt_minstret();

        printf("ITA MatMul cycles = %u\n", end_cycles - start_cycles);
        printf("ITA MatMul instructions = %u\n", end_instructions - start_instructions);

        float ops = (float)MAT_OPS * test_args->repetitions; // MACs
        float ops_per_cycle = (float)ops / (float)(end_cycles - start_cycles);

        tot_err = check(1, (uint8_t *)output_0_buff, (uint8_t *)golden_interm_Pq,
                        SEQUENCE_LENGTH * PROJECTION_SPACE, l1_arena_start,
                        1 * SEQUENCE_LENGTH * PROJECTION_SPACE);

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
