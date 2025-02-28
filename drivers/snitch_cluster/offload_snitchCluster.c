// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Moritz Scherer <scheremo@iis.ee.ethz.ch>
// Philip Wiese <wiesep@iis.ee.ethz.ch>

#include "soc.h"
#include "offload_snitchCluster.h"

#include <stdint.h>
#include <stddef.h>

/** \addtogroup cluster
 *  @{
 */

// Persistent trampoline function pointer for each core
void (*_trampoline_function[NUM_CLUSTER_CORES])(void *) = {NULL};

// Peristent argument storage for the trampoline function
void *_trampoline_args[NUM_CLUSTER_CORES] = {NULL};

// Persistant stack pointer storage for each core
void *_trampoline_stack[NUM_CLUSTER_CORES] = {NULL};

/**
 * @brief Trampoline function for the cluster core.
 * This function will set up the stack pointer and call the function.
 *
 * @warning Make sure that this function is compiled with ISA for the Snitch cores (RV32IM)
 *
 */
extern void _trampoline();

/**
 * @brief Generate a trampoline function for the cluster core.
 * The trampoline function will set up the stack pointer and call the function.
 *
 * @param core_id ID of the core to offload to
 * @param function Function pointer to offload
 * @param args Arguments to pass to the function
 * @param stack Stack pointer for core
 * @return A pointer to the persistent trampoline function
 */
static void *_generate_trampoline(uint32_t core_id, void (*function)(void *), void *args,
                                  void *stack) {
    // Assign trampoline with captured arguments to the persistent function pointer
    _trampoline_function[core_id] = function;
    _trampoline_args[core_id] = args;
    _trampoline_stack[core_id] = stack;

    // Store captured arguments in a persistent context if needed
    return _trampoline;
}

/**
 * @brief Setup the interrupt handler for the cluster cores.
 * All cores in all clusters will jump to the handler when an interrupt is triggered.
 *
 * @param handler Function pointer to the interrupt handler
 */
void setup_snitchCluster_interruptHandler(void *handler) {
    volatile void **snitchTrapHandlerAddr =
        (volatile void **)(SOC_CTRL_BASE + CHIMERA_SNITCH_INTR_HANDLER_ADDR_REG_OFFSET);

    *snitchTrapHandlerAddr = handler;
}

/**
 * @brief Offload a void function pointer to a cluster's core.
 * The function will be executed on the specified core of the cluster.
 *
 * @param function Function pointer to offload
 * @param args Arguments to pass to the function
 * @param stack_ptr Stack pointer for the core
 * @param clusterId ID of the cluster to offload to
 * @param core_id ID of the core to offload to (cores are 0-indexed for each cluster)
 */
void offload_snitchCluster_core(void *function, void *args, void *stack_ptr, uint8_t clusterId,
                                uint32_t core_id) {
    volatile void **snitchBootAddr =
        (volatile void **)(SOC_CTRL_BASE + CHIMERA_SNITCH_BOOT_ADDR_REG_OFFSET);

    // Core with hartid 0 is CVA6's, thus we start with 1
    uint32_t hartId = 1 + core_id;
    for (uint32_t i = 0; i < clusterId; i++) {
        hartId += _chimera_numCores[i];
    }

    *snitchBootAddr = _generate_trampoline(hartId, function, args, stack_ptr);

    // Check if the cluster is busy
    wait_snitchCluster_busy(clusterId);

    // Send interrupt to the core
    volatile uint32_t *interruptTarget = ((uint32_t *)CLINT_CTRL_BASE) + hartId;
    *interruptTarget = 1;
}

/**
 * @brief Offload a void function pointer to a cluster.
 * The function will be executed on all cores of the cluster.
 *
 * @param function Function pointer to offload
 * @param args Arguments to pass to the function
 * @param stack_ptr Stack pointer for the core
 * @param clusterId ID of the cluster to offload to
 */
void offload_snitchCluster(void *function, void *args, void *stack_ptr, uint8_t clusterId) {
    volatile void **snitchBootAddr =
        (volatile void **)(SOC_CTRL_BASE + CHIMERA_SNITCH_BOOT_ADDR_REG_OFFSET);

    // Core with hartid 0 is CVA6's, thus we start with 1
    uint32_t hartId = 1;
    for (uint32_t i = 0; i < clusterId; i++) {
        hartId += _chimera_numCores[i];
    }

    // Check if the cluster is busy
    wait_snitchCluster_busy(clusterId);

    for (uint32_t i = 0; i < _chimera_numCores[clusterId]; i++) {
        *snitchBootAddr = _generate_trampoline(hartId, function, args, stack_ptr);
        // Send interrupt to the core
        volatile uint32_t *interruptTarget = ((uint32_t *)CLINT_CTRL_BASE) + hartId + i;
        *interruptTarget = 1;
    }
}

/**
 * @brief Blocking wait for the cluster to become idle.
 * The function busy waits until the cluster is ready.
 *
 * @warning In the current Snitch bootrom implementation each cores clears the busy flag as soon as
 * is returned. Hence the busy flag does not reflect the actual status of the cluster.
 *
 * @todo Fix the bootrom after adding synchornization primitives for the Snitch cores.
 *
 * @param clusterId ID of the cluster to wait for.
 */
void wait_snitchCluster_busy(uint8_t clusterId) {
    volatile int32_t *busy_ptr;

    if (clusterId == 0) {
        busy_ptr = (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_BUSY_REG_OFFSET);
    } else if (clusterId == 1) {
        busy_ptr = (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_1_BUSY_REG_OFFSET);
    } else if (clusterId == 2) {
        busy_ptr = (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_2_BUSY_REG_OFFSET);
    } else if (clusterId == 3) {
        busy_ptr = (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_3_BUSY_REG_OFFSET);
    } else if (clusterId == 4) {
        busy_ptr = (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_4_BUSY_REG_OFFSET);
    }

    while (*busy_ptr == 1) {
    }
    // TODO: temporary race condition fix
    for (int i = 0; i < 100; i++) {
        // NOP
        asm volatile("addi x0, x0, 0\n" :::);
    }

    return;
}

/**
 * @brief Wait for the cluster to return a value.
 * The function busy waits until the cluster returns a non-zero value.
 *
 * @warning The return values must be non-zero, otherwise the function will busy wait forever!
 *
 * @param clusterId ID of the cluster to wait for.
 * @return uint32_t Return value of the cluster.
 */
uint32_t wait_snitchCluster_return(uint8_t clusterId) {
    volatile int32_t *snitchReturnAddr;
    if (clusterId == 0) {
        snitchReturnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CLUSTER_0_RETURN_REG_OFFSET);
    } else if (clusterId == 1) {
        snitchReturnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CLUSTER_1_RETURN_REG_OFFSET);
    } else if (clusterId == 2) {
        snitchReturnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CLUSTER_2_RETURN_REG_OFFSET);
    } else if (clusterId == 3) {
        snitchReturnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CLUSTER_3_RETURN_REG_OFFSET);
    } else if (clusterId == 4) {
        snitchReturnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CLUSTER_4_RETURN_REG_OFFSET);
    }

    while (*snitchReturnAddr == 0) {
    }

    uint32_t retVal = *snitchReturnAddr;
    *snitchReturnAddr = 0;

    return retVal;
}

/** @}*/
