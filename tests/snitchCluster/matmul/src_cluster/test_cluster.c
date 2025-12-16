// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>
#include <string.h>

// Include Application Headers
#include "test_cluster.h"
#include "test_host.h"

// #include "testinputs_32x48x64.h"
// #include "testoutputs_32x48x64.h"

#include "testinputs_16x16x16.h"
#include "testoutputs_16x16x16.h"

// Include Target Specific Headers
#include "soc.h"

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

void MatMul_unrolled_2x2_parallel_s8_rv32im(int8_t const *__restrict__ pSrcA,
                                            int8_t const *__restrict__ pSrcB,
                                            int32_t *__restrict__ pDstC, uint32_t M, uint32_t N,
                                            uint32_t P) {

    uint32_t core_id = snrt_cluster_core_idx();
    uint32_t numThreads = snrt_cluster_compute_core_num();

    /*
     * Small tiled 2x2 matrix multiplication kernel.
     *
     * Parallelization and layout notes:
     * - Matrices are A (M x N), B (N x P), C (M x P).
     * - Each compute core works on a subset of the output rows (two rows per tile)
     *   and a contiguous chunk of columns. The algorithm is unrolled in 2x2 tiles
     *   to reduce loop overhead and improve instruction-level parallelism.
     * - The kernel assumes M, N and P are multiples of 2. No bounds-checking is
     *   performed to keep the inner loops fast (caller responsibility).
     */

    // Determine work distribution for this core
    uint32_t const c = 1; // number of column partitions (kept 1 here for simplicity)
    uint32_t const c_start = (P / c) * (core_id % c);
    uint32_t const c_end = (P / c) * ((core_id % c) + 1);

    // Each core processes two rows at a time. Iterate over assigned rows and cols.
    for (uint32_t i = 2 * (core_id / c); i < M; i += 2 * (numThreads / c)) {
        for (uint32_t j = c_start; j < c_end; j += 2) {
            int32_t c00 = 0;
            int32_t c01 = 0;
            int32_t c10 = 0;
            int32_t c11 = 0;

            // k loops over the shared dimension N in steps of 2 to match the 2x2
            // unrolling. Local temporaries are loaded first to encourage the
            // compiler to keep values in registers and improve scheduling.
            for (uint32_t k = 0; k < N; k += 2) {
                int8_t val_a00 = (int8_t)(pSrcA[(i + 0) * N + k + 0]);
                int8_t val_a01 = (int8_t)(pSrcA[(i + 0) * N + k + 1]);
                int8_t val_a10 = (int8_t)(pSrcA[(i + 1) * N + k + 0]);
                int8_t val_a11 = (int8_t)(pSrcA[(i + 1) * N + k + 1]);
                int8_t val_b00 = (int8_t)(pSrcB[(k + 0) * P + j + 0]);
                int8_t val_b01 = (int8_t)(pSrcB[(k + 0) * P + j + 1]);
                int8_t val_b10 = (int8_t)(pSrcB[(k + 1) * P + j + 0]);
                int8_t val_b11 = (int8_t)(pSrcB[(k + 1) * P + j + 1]);

                c00 += val_a00 * val_b00;
                c00 += val_a01 * val_b10;
                c01 += val_a00 * val_b01;
                c01 += val_a01 * val_b11;
                c10 += val_a10 * val_b00;
                c10 += val_a11 * val_b10;
                c11 += val_a10 * val_b01;
                c11 += val_a11 * val_b11;
            }

            // Store the computed 2x2 tile into the output matrix C
            pDstC[(i + 0) * P + j + 0] = c00;
            pDstC[(i + 0) * P + j + 1] = c01;
            pDstC[(i + 1) * P + j + 0] = c10;
            pDstC[(i + 1) * P + j + 1] = c11;
        }
    }
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
 *
 * @brief Main function of the cluster test.
 *
 */
int32_t testReturn(void *args) {
    argCluster_t *retVal = (argCluster_t *)args;

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

        printf("Starting MatMul execution (%ux%u x %ux%u = %ux%u)...\n", MAT_M, MAT_N, MAT_N, MAT_P,
               MAT_M, MAT_P);
    }

    snrt_cluster_hw_barrier();

    uint32_t start_cycles = 0, end_cycles = 0;
    /*
     * Start cycle counter before compute. Only compute cores execute the
     * MatMul kernel.
     */
    start_cycles = snrt_mcycle();
    if (snrt_is_compute_core()) {
        MatMul_unrolled_2x2_parallel_s8_rv32im(DeeployNetwork_input_0, DeeployNetwork_input_1,
                                               DeeployNetwork_output_0, MAT_M, MAT_N, MAT_P);
    }
    end_cycles = snrt_mcycle();
    snrt_cluster_hw_barrier();
    printf("RQGemm cycles = %u\n", end_cycles - start_cycles);
    snrt_cluster_hw_barrier();

    /* Each compute core computes its contribution to ops/cycle and updates
     * the shared accumulator atomically. The printed ops/cycle/core value is
     * per-core.
     */
    if (snrt_is_compute_core()) {
        float ops = ((float)MAT_OPS) / snrt_cluster_compute_core_num(); // MACs
        float ops_per_cycle_per_core = (float)ops / (float)(end_cycles - start_cycles);
        printf("RQGemm ops/cycle/core = %.6f\n", ops_per_cycle_per_core);

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
        for (uint32_t i = 0; i < (MAT_M * MAT_P); i++) {
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
        retVal->errors = tot_err;
        retVal->runtime_cycles = end_cycles - start_cycles;
        retVal->ops_per_cycle = (uint32_t)(ops_per_cycle * 1e6);
    }

    snrt_cluster_hw_barrier();

    return tot_err << 1;
}