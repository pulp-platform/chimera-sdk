// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/** \addtogroup device_snitchCluster
 *  @{
 */

#include <stdint.h>

#include "trampoline_snitchCluster.h"
#include "soc.h"

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
    _SETUP_GP();

    asm volatile(
        // Get hart ID (hardware thread ID)
        "csrr t0, mhartid\n" // Load mhartid into a0

        "la a0, _trampoline_stack\n"    // Load address of _trampoline_stack
        "addi t0, t0, -%[hartOffset]\n" // Subtract cluster hart base offset
        "slli t0, t0, 2\n"              // Multiply hart ID by 4 (size of pointer)
        "add a0, a0, t0\n"              // Compute the address of _trampoline_stack[hartId]
        "lw sp, 0(a0)\n"                // Load stack pointer from the computed address

        // Adjust stack pointer for thread-local data (.tdata section)
        "la t1, __tdata_end\n"   // Load end address of .tdata section into t0
        "la t2, __tdata_start\n" // Load start address of .tdata section into t1
        "sub t1, t1, t2\n"       // Compute size of .tdata section
        "sub sp, sp, t1\n"       // Allocate space for .tdata on stack

        // Adjust stack pointer for thread-local BSS (.tbss section)
        "la t1, __tbss_end\n"   // Load end address of .tbss section into t0
        "la t2, __tbss_start\n" // Load start address of .tbss section into t1
        "sub t1, t1, t2\n"      // Compute size of .tbss section
        "sub sp, sp, t1\n"      // Allocate space for .tbss on stack

        // Set thread pointer (tp) to stack pointer
        "mv tp, sp\n" // Move stack pointer to thread pointer

        // Align stack pointer to 16-byte boundary after allocating TLS
        "andi sp, sp, -0xF\n" // Ensure stack pointer is 16-Byte aligned (ABI)

        // Load function pointer and arguments
        "la a0, _trampoline_function\n" // Load address of _trampoline_function
        "add a0, a0, t0\n"              // Compute address of _trampoline_function[hartId]
        "lw a1, 0(a0)\n"                // Load function pointer into a1

        "la a0, _trampoline_args\n" // Load address of _trampoline_args
        "add a0, a0, t0\n"          // Compute address of _trampoline_args[hartId]
        "lw a0, 0(a0)\n"            // Load argument pointer into a0

        // Call the offloaded function
        "jr a1\n" // Jump and link to the function pointer in a1
        :         /* No outputs */
        : [hartOffset] "i"(CLUSTER_HART_BASE));
}

/** @}*/