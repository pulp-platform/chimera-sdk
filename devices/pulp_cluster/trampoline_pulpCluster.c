// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Sergio Mazzola <smazzola@iis.ee.ethz.ch>

/** \addtogroup devices_pulpCluster
 *  @{
 */

#include <stdint.h>

#include "soc.h"
#include "trampoline_pulpCluster.h"

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
 * @warning Make sure that this function is compiled with ISA for the PULP Cluster cores
 *
 */
// WIESEP: Make sure the compiler does not allocate a stack frame
void __attribute__((naked)) _trampoline() {
    _SETUP_GP_TP();

    asm volatile(
        // Make sure to properly initialize latch based RF
        "lui x1, 0\n"
        "lui x2, 0\n"
        "lui x3, 0\n"
        "lui x4, 0\n"
        "lui x5, 0\n"
        "lui x6, 0\n"
        "lui x7, 0\n"
        "lui x8, 0\n"
        "lui x9, 0\n"
        "lui x10, 0\n"
        "lui x11, 0\n"
        "lui x12, 0\n"
        "lui x13, 0\n"
        "lui x14, 0\n"
        "lui x15, 0\n"
        "lui x16, 0\n"
        "lui x17, 0\n"
        "lui x18, 0\n"
        "lui x19, 0\n"
        "lui x20, 0\n"
        "lui x21, 0\n"
        "lui x22, 0\n"
        "lui x23, 0\n"
        "lui x24, 0\n"
        "lui x25, 0\n"
        "lui x26, 0\n"
        "lui x27, 0\n"
        "lui x28, 0\n"
        "lui x29, 0\n"
        "lui x30, 0\n"
        "lui x31, 0\n"

        // Get hart ID (hardware thread ID)
        "csrr t1, mhartid\n" // Load mhartid into a0

        // Set up stack pointer
        "la a0, _trampoline_stack\n" // Load address of _trampoline_stack
        "slli t1, t1, 2\n"           // Multiply hart ID by 4 (size of pointer)
        "add a0, a0, t1\n"           // Compute the address of _trampoline_stack[hart_id]
        "lw sp, 0(a0)\n"             // Load stack pointer from the computed address

        // Load function pointer and arguments
        "la a0, _trampoline_function\n" // Load address of _trampoline_function
        "add a0, a0, t1\n"              // Compute address of _trampoline_function[hart_id]
        "lw a1, 0(a0)\n"                // Load function pointer into a1

        "la a0, _trampoline_args\n" // Load address of _trampoline_args
        "add a0, a0, t1\n"          // Compute address of _trampoline_args[hart_id]
        "lw a0, 0(a0)\n"            // Load argument pointer into a0
        
        "call set_busy\n"

        // Call the offloaded function
        "jr a1\n" // Jump and link to the function pointer in a1
        
        "call clean_busy\n"
        // By calling immediately after return, register contents in a0 are passed as the first argument
        "call cluster_return\n"
    );
}

/**
 * @brief Set the busy flag of the cluster.
 * Only core 0 of each cluster sets the busy flag.
 */
void set_busy() {

    uint8_t hart_id;
    asm("csrr %0, mhartid" : "=r"(hart_id)::);

    switch (hart_id) {

    case 1:
        *((volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_BUSY_REG_OFFSET)) = 1;
        break;
    case 1 + CLUSTER_0_NUMCORES:
        *((volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_1_BUSY_REG_OFFSET)) = 1;
        break;
    case 1 + CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES:
        *((volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_2_BUSY_REG_OFFSET)) = 1;
        break;
    case 1 + CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES + CLUSTER_2_NUMCORES:
        *((volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_3_BUSY_REG_OFFSET)) = 1;
        break;
    case 1 + CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES + CLUSTER_2_NUMCORES + CLUSTER_3_NUMCORES:
        *((volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_4_BUSY_REG_OFFSET)) = 1;
        break;
    }

    return;
}

/**
 * @brief Clean the busy flag of the cluster.
 * Only core 0 of each cluster cleans the busy flag.
 */
void clean_busy() {

    uint8_t hart_id;
    asm("csrr %0, mhartid" : "=r"(hart_id)::);

    switch (hart_id) {

    case 1:
        *((volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_BUSY_REG_OFFSET)) = 0;
        break;
    case 1 + CLUSTER_0_NUMCORES:
        *((volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_1_BUSY_REG_OFFSET)) = 0;
        break;
    case 1 + CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES:
        *((volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_2_BUSY_REG_OFFSET)) = 0;
        break;
    case 1 + CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES + CLUSTER_2_NUMCORES:
        *((volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_3_BUSY_REG_OFFSET)) = 0;
        break;
    case 1 + CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES + CLUSTER_2_NUMCORES + CLUSTER_3_NUMCORES:
        *((volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_4_BUSY_REG_OFFSET)) = 0;
        break;
    }

    return;
}

/**
 * @brief Captures the return value of the cluster.
 * The return value of the cluster is decided only by core 0 of each cluster.
 *
 * @param ret Return value of the cluster core. To signal that the cluster has returned, the least
 * significant bit is set to 1 (e.g., to avoid the loss of the LSB, @param ret is supposed come
 * already shifted by 1).
 */
void cluster_return(uint32_t ret) {

    uint32_t retVal = ret | 0x000000001;

    uint8_t hart_id;
    asm("csrr %0, mhartid" : "=r"(hart_id)::);

    switch (hart_id) {

    case 1:
        *((volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_RETURN_REG_OFFSET)) =
            retVal;
        break;
    case 1 + CLUSTER_0_NUMCORES:
        *((volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_1_RETURN_REG_OFFSET)) =
            retVal;
        break;
    case 1 + CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES:
        *((volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_2_RETURN_REG_OFFSET)) =
            retVal;
        break;
    case 1 + CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES + CLUSTER_2_NUMCORES:
        *((volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_3_RETURN_REG_OFFSET)) =
            retVal;
        break;
    case 1 + CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES + CLUSTER_2_NUMCORES + CLUSTER_3_NUMCORES:
        *((volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_4_RETURN_REG_OFFSET)) =
            retVal;
        break;
    }

    return;
}

/** @}*/
