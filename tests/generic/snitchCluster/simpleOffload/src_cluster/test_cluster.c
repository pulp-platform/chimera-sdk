// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Moritz Scherer <scheremo@iis.ee.ethz.ch>

#include "test_cluster.h"
#include "test_host.h"

#include "soc.h"

static uint32_t *clintPointer = (uint32_t *)CLINT_CTRL_BASE;

/**
 * @brief Interrupt handler for the cluster, which clears the interrupt flag for the current hart.
 *
 * @warning Stack, thread and global pointer might not yet be set up!
 */
__attribute__((naked)) void clusterInterruptHandler() {
    asm volatile(
        // Load global pointer
        ".option push\n"
        ".option norelax\n"          // Disable relaxation to ensure `la` behaves as expected
        "la gp, __global_pointer$\n" // Load address of global pointer
        ".option pop\n"

        // Set thread pointer (tp) to zero
        "mv tp, zero\n"

        // Load mhartid CSR into t0
        "csrr t0, mhartid\n"
        // Load the base address of clintPointer into t1
        "lw t1, %0\n"
        // Calculate the interrupt target address: t1 = t1 + (t0 * 4)
        "slli t0, t0, 2\n"
        "add t1, t1, t0\n"
        // Store 0 to the interrupt target address
        "sw zero, 0(t1)\n"
        "ret"
        :
        : "m"(clintPointer) // Pass clintPointer as input
        : "t0", "t1"        // Declare clobbered registers
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