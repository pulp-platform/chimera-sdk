// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Include Application Headers
#include "test_cluster.h"
#include "test_host.h"

#include "testdata_16x384.h"

// Include Target Specific Headers
#include "soc.h"
#include "test.h"

// Include Driver Headers
#include "macros.h"
#include "trampoline.h"

// Include Runtime Headers
#include "snrt.h"

/**
 * @brief Interrupt handler for the cluster, which clears the interrupt flag for the current hart.
 *
 * @warning Stack, thread and global pointer might not yet be set up!
 */
__attribute__((naked)) KEEP void clusterInterruptHandler() {
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

#define CLAMP(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

// JUNGVI: This implementation assumes that the kernel is not tiled over the channels
void SnitchRequantShift_s8_s8_NHWC(int8_t *data_in, int32_t size, int32_t *mul, int32_t *add,
                                   int8_t *data_out, int32_t log2D, int32_t channels) {

    uint32_t core_id = snrt_cluster_core_idx();
    uint32_t numThreads = snrt_cluster_compute_core_num();

    int32_t num_rows = size / channels;

    uint32_t chunkSize = num_rows / numThreads;
    uint32_t start = core_id * chunkSize;
    uint32_t stop = (core_id == (numThreads - 1)) ? num_rows : start + chunkSize;

    int32_t data_idx = start * channels;

    int32_t intermediate;
    int8_t out;

    for (int j = start; j < stop; j++) {
        for (int i = 0; i < channels; i++) {
            intermediate = ((int32_t)data_in[data_idx]) * mul[i] + add[i];
            intermediate = ((intermediate + ((1 << (log2D - 1)))) >> log2D);
            out = (int8_t)CLAMP(intermediate, -128, 127);
            data_out[data_idx] = out;
            data_idx++;
        }
    }
}

/*
 * Global tensor pointers placed in a cluster-visible section (.cbss).
 * These are assigned on the DM (data-mover) core and then used by compute
 * cores.
 */
SNRT_CLUSTER_L1_ZERO(int8_t *DeeployNetwork_input_0);
SNRT_CLUSTER_L1_ZERO(int8_t *DeeployNetwork_output_0);

/*
 * Accumulator for ops-per-cycle computed by each compute core and atomically
 * added to this shared float in .cdata.
 */
SNRT_CLUSTER_L1_COPY(static float ops_per_cycle) = 0.0f;

/**
 * @brief Main function of the cluster test.
 *
 * @return int Return 0 if the test was successful, -1 otherwise.
 */
KEEP int32_t layerscale_l1_test(void *args) {
    test_cluster_args_t *test_args = (test_cluster_args_t *)args;
    test_cluster_result_t *test_retVal = (test_cluster_result_t *)(test_args->result);

    /*
     * Initialize the Snitch runtime.
     */
    snrt_init();

    /*
     * DM core (data master) performs data setup and L1 allocation. This keeps
     * high-latency operations out of the compute cores and centralizes
     * memory management.
     */
    if (snrt_is_dm_core()) {
        /*
         * Assign pointers to input data (presumably located in L2/flash)
         * and allocate an L1 buffer for the output matrix. The compute cores
         * will read the inputs and write into the L1-allocated output.
         */
        DeeployNetwork_input_0 = input_tensor;
        DeeployNetwork_output_0 = (int8_t *)snrt_l1_alloc(sizeof(expected_output));

        printf("Starting LayerScale execution (%ux%u) for %d iterations\n", LAYERSCALE_M,
               LAYERSCALE_N, test_args->repetitions);
    }

    snrt_cluster_hw_barrier();

    uint32_t start_cycles = 0, end_cycles = 0;
    uint32_t start_instructions = 0, end_instructions = 0;
    /*
     * Start cycle counter before compute. Only compute cores execute the
     * MatMul kernel.
     */
    start_cycles = snrt_mcycle();
    start_instructions = snrt_minstret();
    for (int i = 0; i < test_args->repetitions; i++) {
        if (snrt_is_dm_core() && (i == test_args->repetitions - 1)) {
            // Do one more iteration after clearning the output
            // to make sure the results are still correct
            snrt_dma_start_1d((void *)DeeployNetwork_output_0, (void *)snrt_zero_memory_ptr(),
                              (LAYERSCALE_M * LAYERSCALE_N));

            snrt_dma_wait_all();
        }
        snrt_cluster_hw_barrier();
        if (snrt_is_compute_core()) {
            SnitchRequantShift_s8_s8_NHWC(DeeployNetwork_input_0, 6144, mul_tensor, add_tensor,
                                          DeeployNetwork_output_0, 16, 1);
        }
    }
    end_cycles = snrt_mcycle();
    end_instructions = snrt_minstret();
    snrt_cluster_hw_barrier();
    printf("LayerScale cycles = %u\n", end_cycles - start_cycles);
    printf("LayerScale instructions = %u\n", end_instructions - start_instructions);
    snrt_cluster_hw_barrier();

    /* Each compute core computes its contribution to ops/cycle and updates
     * the shared accumulator atomically. The printed ops/cycle/core value is
     * per-core.
     */
    if (snrt_is_compute_core()) {
        float ops = ((float)LAYERSCALE_OPS * test_args->repetitions) /
                    snrt_cluster_compute_core_num(); // MACs
        float ops_per_cycle_per_core = (float)ops / (float)(end_cycles - start_cycles);
        printf("LayerScale ops/cycle/core = %.6f\n", ops_per_cycle_per_core);

        __atomic_add_fetch(&ops_per_cycle, ops_per_cycle_per_core, __ATOMIC_RELAXED);
    }
    snrt_cluster_hw_barrier();

    if (snrt_cluster_core_idx() == 0) {
        printf("Total ops/cycle = %.6f\n", ops_per_cycle);
    }
    snrt_cluster_hw_barrier();

    /*
     * Verification: core 0 compares the L1-resident output with a reference
     * golden buffer `testOutputVector0`.  The function returns (tot_err << 1)
     * to encode the error count in the return value.
     */
    int32_t tot_err = 0;
    if (snrt_cluster_core_idx() == 0) {
        int32_t diff;
        int32_t expected, actual;
        for (uint32_t i = 0; i < (LAYERSCALE_M * LAYERSCALE_N); i++) {
            expected = expected_output[i];
            actual = DeeployNetwork_output_0[i];
            diff = expected - actual;
            if (diff) {
                tot_err += 1;
                printf("Expected: %4d  Actual: %4d  Diff: %4d at Index %12u\n", expected, actual,
                       diff, i);
            }
        }
        if (tot_err != 0) {
            printf("Test failed with %d errors\r\n", tot_err);
        }
        test_retVal->errors = tot_err;
        test_retVal->runtime_cycles = end_cycles - start_cycles;
        test_retVal->ops_per_cycle = (uint32_t)(ops_per_cycle * 1e6);
    }

    snrt_cluster_hw_barrier();

    return tot_err << 1;
}