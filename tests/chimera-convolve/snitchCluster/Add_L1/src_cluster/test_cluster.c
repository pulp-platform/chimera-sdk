// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>
#include <string.h>

// Include Application Headers
#include "test_cluster.h"
#include "test_host.h"

#include "testdata_128x128.h"

// Include Target Specific Headers
#include "soc.h"
#include "test.h"

// Include Driver Headers
#include "trampoline_snitchCluster.h"

// Include Runtime Headers
#include "snrt.h"

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

void SnitchAdd(int8_t *pIn1, int8_t *pIn2, int32_t *pOut, uint32_t size) {

    uint32_t core_id = snrt_cluster_core_idx();
    uint32_t numThreads = snrt_cluster_compute_core_num();
    uint32_t chunk, chunkSize, start, stop;

    chunkSize = size / numThreads;
    start = core_id * chunkSize;
    stop = (core_id == (numThreads - 1)) ? size : start + chunkSize;

#pragma loopunroll 16
    for (uint32_t i = start; i < stop; i++) {
        pOut[i] = pIn1[i] + pIn2[i];
    }

    //     int i;
    // #pragma loopunroll 4
    // for (i = start; i < stop; i+=4) {
    //         int32_t in1_packed = *(int32_t *)&pIn1[i];
    //         int32_t in2_packed = *(int32_t *)&pIn2[i];
    //         int32_t out_packed;
    //         out_packed = in1_packed + in2_packed;
    //         pOut[i] = (out_packed >> 0) & 0xFF;
    //         pOut[i+1] = (out_packed >> 8) & 0xFF;
    //         pOut[i+2] = (out_packed >> 16) & 0xFF;
    //         pOut[i+3] = (out_packed >> 24) & 0xFF;
    // }
    // for (; i < stop; i++) {
    //         pOut[i] = pIn1[i] + pIn2[i];
    // }
}

/*
 * Global tensor pointers placed in a cluster-visible section (.cbss).
 * These are assigned on the DM (data-mover) core and then used by compute
 * cores.
 */
SNRT_CLUSTER_L1_ZERO(int8_t *DeeployNetwork_input_0);
SNRT_CLUSTER_L1_ZERO(int8_t *DeeployNetwork_input_1);
SNRT_CLUSTER_L1_ZERO(int32_t *DeeployNetwork_output_0);

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
int32_t add_l1_test(void *args) {
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
        DeeployNetwork_input_0 = testInputVector0;
        DeeployNetwork_input_1 = testInputVector1;
        DeeployNetwork_output_0 = (int32_t *)snrt_l1_alloc(sizeof(testOutputVector0));

        printf("Starting Add execution (%ux%u) for %d iterations\n", ADD_M, ADD_N,
               test_args->repetitions);
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
                              (ADD_M * ADD_N));

            snrt_dma_wait_all();
        }
        snrt_cluster_hw_barrier();
        if (snrt_is_compute_core()) {
            SnitchAdd(DeeployNetwork_input_0, DeeployNetwork_input_1, DeeployNetwork_output_0,
                      16384);
        }
    }
    end_cycles = snrt_mcycle();
    end_instructions = snrt_minstret();
    snrt_cluster_hw_barrier();
    printf("Add cycles = %u\n", end_cycles - start_cycles);
    printf("Add instructions = %u\n", end_instructions - start_instructions);
    snrt_cluster_hw_barrier();

    /* Each compute core computes its contribution to ops/cycle and updates
     * the shared accumulator atomically. The printed ops/cycle/core value is
     * per-core.
     */
    if (snrt_is_compute_core()) {
        float ops =
            ((float)ADD_OPS * test_args->repetitions) / snrt_cluster_compute_core_num(); // MACs
        float ops_per_cycle_per_core = (float)ops / (float)(end_cycles - start_cycles);
        printf("Add ops/cycle/core = %.6f\n", ops_per_cycle_per_core);

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
    int32_t tot_err = -1;
    if (snrt_cluster_core_idx() == 0) {
        int32_t diff;
        int32_t expected, actual;
        for (uint32_t i = 0; i < (ADD_M * ADD_N); i++) {
            expected = testOutputVector0[i];
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