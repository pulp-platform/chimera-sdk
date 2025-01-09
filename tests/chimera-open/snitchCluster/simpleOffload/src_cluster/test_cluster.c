// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Moritz Scherer <scheremo@iis.ee.ethz.ch>

#include "test_cluster.h"
#include "test_host.h"

#include "soc.h"

#include "trampoline_snitchCluster.h"

/**
 * @brief Interrupt handler for the cluster, which clears the interrupt flag for the current hart.
 *
 * @warning Stack, thread and global pointer might not yet be set up!
 */
__attribute__((naked)) void clusterInterruptHandler() {
    _SETUP_GP_TP();

    asm volatile(
        // Load mhartid CSR into t0
        "csrr t0, mhartid\n"
        // Load the base address of clintPointer into t1
        // "lw t1, %0\n"

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
 * @brief Main function of the cluster test.
 *
 * @return int Return 0 if the test was successful, -1 otherwise.
 */
int32_t testReturn(void *args) {
    // Cast to the correct struct
    offloadArgs_t *argsStruct = (offloadArgs_t *)args;

    // Check if the value is correct
    if (argsStruct->value != 0xdeadbeef) {
        return -1;
    }

    return TESTVAL;
}