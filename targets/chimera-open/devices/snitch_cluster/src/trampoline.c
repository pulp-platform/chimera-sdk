// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/** \addtogroup device_snitchCluster
 *  @{
 */

#include <stdint.h>
#include <stddef.h>

#include "macros.h"
#include "trampoline.h"
#include "shared.h"
#include "soc.h"

/**
 * @brief Trampoline function for the cluster core.
 * This function will set up the stack pointer and call the offloaded function.
 *
 * Stack pointer, function pointer, and argument are read from the shared_data
 * struct in the .common section (shared between host and device).
 *
 * @warning Make sure that this function is compiled with ISA for the Snitch
 * cores (RV32IM).
 */
// WIESEP: Make sure the compiler does not allocate a stack frame
__attribute__((naked)) KEEP void trampoline() {
    _SETUP_GP();

    asm volatile(
        // Turn interrupts off, so later wake-ups leave the bootrom's wfi without a trap
        "csrci mstatus, 8\n"

        // Get hart ID (hardware thread ID)
        "csrr t0, mhartid\n"

        // Clear this core's software interrupt
        "la t1, __base_clint\n"
        "slli t2, t0, 2\n"
        "add t1, t1, t2\n"
        "sw zero, 0(t1)\n"

        // ---- Load stack pointer from shared_data.trampoline_stack[hartId] ----
        "la a0, shared_data\n"
        "addi a0, a0, %[off_stack]\n"   // a0 = &shared_data.trampoline_stack
        "addi t0, t0, -%[hartOffset]\n" // t0 = hartId - CLUSTER_HART_BASE
        "slli t0, t0, 2\n"              // t0 *= 4 (uint32_t index)
        "add a0, a0, t0\n"              // a0 = &trampoline_stack[hartId]
        "lw sp, 0(a0)\n"                // sp = trampoline_stack[hartId]

        // Adjust stack pointer for thread-local data (.tdata section)
        "la t1, __tdata_end\n"
        "la t2, __tdata_start\n"
        "sub t1, t1, t2\n"
        "sub sp, sp, t1\n"

        // Adjust stack pointer for thread-local BSS (.tbss section)
        "la t1, __tbss_end\n"
        "la t2, __tbss_start\n"
        "sub t1, t1, t2\n"
        "sub sp, sp, t1\n"

        // Set thread pointer (tp) to stack pointer
        "mv tp, sp\n"

        // Align stack pointer to 16-byte boundary after allocating TLS
        "andi sp, sp, -0xF\n"

        // ---- Load function pointer from shared_data.trampoline_function[hartId] ----
        "la a1, shared_data\n"
        "addi a1, a1, %[off_func]\n" // a1 = &shared_data.trampoline_function
        "add a1, a1, t0\n"           // a1 = &trampoline_function[hartId]
        "lw a1, 0(a1)\n"             // a1 = trampoline_function[hartId]

        // ---- Load argument from shared_data.trampoline_args[hartId] ----
        "la a0, shared_data\n"
        "addi a0, a0, %[off_args]\n" // a0 = &shared_data.trampoline_args
        "add a0, a0, t0\n"           // a0 = &trampoline_args[hartId]
        "lw a0, 0(a0)\n"             // a0 = trampoline_args[hartId]

        // ---- Set trampoline_ack[hartId]; t2 = 1 depends on all three loads, so no fence ----
        "la t1, shared_data\n"
        "addi t1, t1, %[off_ack]\n"
        "add t1, t1, t0\n"
        "xor t2, a0, a0\n"
        "xor t3, a1, a1\n"
        "or t2, t2, t3\n"
        "xor t3, sp, sp\n"
        "or t2, t2, t3\n"
        "addi t2, t2, 1\n"
        "sw t2, 0(t1)\n"

        // Call the offloaded function
        "jr a1\n"
        : /* No outputs */
        : [hartOffset] "i"(CLUSTER_HART_BASE),
          [off_stack] "i"(offsetof(chimera_shared_data_t, trampoline_stack)),
          [off_func] "i"(offsetof(chimera_shared_data_t, trampoline_function)),
          [off_args] "i"(offsetof(chimera_shared_data_t, trampoline_args)),
          [off_ack] "i"(offsetof(chimera_shared_data_t, trampoline_ack)));
}

/** @}*/
