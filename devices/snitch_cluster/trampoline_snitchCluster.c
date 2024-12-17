// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Philip Wiese <wiesep@iis.ee.ethz.ch>

/** \addtogroup devices_snitchCluster
 *  @{
 */

#include <stdint.h>

// Persistent trampoline function pointer for each core
extern void (*_trampoline_function)(void *);

// Peristent argument storage for the trampoline function
extern void *_trampoline_args;

// Persistant stack pointer storage for each core
extern void *_trampoline_stack;

/**
 * @brief Trampoline function for the cluster core.
 * This function will set up the stack pointer and call the function.
 *
 * @warning Make sure that this function is compiled with ISA for the Snitch cores (RV32IM)
 *
 */
// WIESEP: Make sure the compiler does not allocate a stack frame
void __attribute__((naked)) _trampoline() {
    asm volatile(
        // Get hart ID (hardware thread ID)
        "csrr t1, mhartid\n" // Load mhartid into a0

        // Load global pointer
        ".option push\n"
        ".option norelax\n"          // Disable relaxation to ensure `la` behaves as expected
        "la gp, __global_pointer$\n" // Load address of global pointer
        ".option pop\n"

        // Set thread pointer (tp) to zero
        "mv tp, zero\n"

        // Set up stack pointer
        "la a0, _trampoline_stack\n" // Load address of _trampoline_stack
        "slli t1, t1, 2\n"           // Multiply hart ID by 4 (size of pointer)
        "add a0, a0, t1\n"           // Compute the address of _trampoline_stack[hartId]
        "lw sp, 0(a0)\n"             // Load stack pointer from the computed address

        // Load function pointer and arguments
        "la a0, _trampoline_function\n" // Load address of _trampoline_function
        "add a0, a0, t1\n"              // Compute address of _trampoline_function[hartId]
        "lw a1, 0(a0)\n"                // Load function pointer into a1

        "la a0, _trampoline_args\n" // Load address of _trampoline_args
        "add a0, a0, t1\n"          // Compute address of _trampoline_args[hartId]
        "lw a0, 0(a0)\n"            // Load argument pointer into a0

        // Call the offloaded function
        "jr a1\n" // Jump and link to the function pointer in a1
    );
}

/** @}*/