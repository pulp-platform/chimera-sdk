// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Sergio Mazzola <smazzola@iis.ee.ethz.ch>

#include "soc.h"
#include "offload_pulpCluster.h"

#include "addr_map.h"

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
 * @warning Make sure that this function is compiled with ISA for the PULP Cluster cores
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
 * @brief Offload a void function pointer to a cluster's core.
 * The function will be executed on the specified core of the cluster.
 *
 * @param function Function pointer to offload
 * @param args Arguments to pass to the function
 * @param stack_ptr Stack pointer for the core
 * @param cluster_id ID of the cluster to offload to
 * @param core_id ID of the core to offload to (cores are 0-indexed for each cluster)
 */
void offload_pulpCluster_core(void *function, void *args, void *stack_ptr, uint8_t cluster_id,
                                uint32_t core_id) {

    // Core with hart_id 0 is CVA6's, thus we start with 1
    uint32_t hart_id = 1 + core_id;
    for (uint32_t i = 0; i < cluster_id; i++) {
        hart_id += _chimera_numCores[i];
    }

    // Address of PULP Cluster's boot address register
    volatile void **bootAddrRegAddr;
    // Address of PULP Cluster's fetch-enable register
    volatile uint32_t *fetchEnRegAddr;

    // Compute cluster offset
    switch (cluster_id) {
        case 0:
            bootAddrRegAddr = (volatile void **)(CLUSTER_0_BASE);
            fetchEnRegAddr = (volatile uint32_t *)(CLUSTER_0_BASE + FETCH_EN_OFFSET);
            break;
        case 1:
            bootAddrRegAddr = (volatile void **)(CLUSTER_1_BASE);
            fetchEnRegAddr = (volatile uint32_t *)(CLUSTER_1_BASE + FETCH_EN_OFFSET);
            break;
        case 2:
            bootAddrRegAddr = (volatile void **)(CLUSTER_2_BASE);
            fetchEnRegAddr = (volatile uint32_t *)(CLUSTER_2_BASE + FETCH_EN_OFFSET);
            break;
        case 3:
            bootAddrRegAddr = (volatile void **)(CLUSTER_3_BASE);
            fetchEnRegAddr = (volatile uint32_t *)(CLUSTER_3_BASE + FETCH_EN_OFFSET);
            break;
        case 4:
            bootAddrRegAddr = (volatile void **)(CLUSTER_4_BASE);
            fetchEnRegAddr = (volatile uint32_t *)(CLUSTER_4_BASE + FETCH_EN_OFFSET);
            break;
        default:
            return;
    }
    // Compute core offset
    switch (core_id) {
        case 0:
            bootAddrRegAddr = (volatile void **)((uint8_t *)bootAddrRegAddr + BOOT_ADDR_CORE_0_OFFSET);
            break;
        case 1:
            bootAddrRegAddr = (volatile void **)((uint8_t *)bootAddrRegAddr + BOOT_ADDR_CORE_1_OFFSET);
            break;
        case 2:
            bootAddrRegAddr = (volatile void **)((uint8_t *)bootAddrRegAddr + BOOT_ADDR_CORE_2_OFFSET);
            break;
        case 3:
            bootAddrRegAddr = (volatile void **)((uint8_t *)bootAddrRegAddr + BOOT_ADDR_CORE_3_OFFSET);
            break;
        case 4:
            bootAddrRegAddr = (volatile void **)((uint8_t *)bootAddrRegAddr + BOOT_ADDR_CORE_4_OFFSET);
            break;
        case 5:
            bootAddrRegAddr = (volatile void **)((uint8_t *)bootAddrRegAddr + BOOT_ADDR_CORE_5_OFFSET);
            break;
        case 6:
            bootAddrRegAddr = (volatile void **)((uint8_t *)bootAddrRegAddr + BOOT_ADDR_CORE_6_OFFSET);
            break;
        case 7:
            bootAddrRegAddr = (volatile void **)((uint8_t *)bootAddrRegAddr + BOOT_ADDR_CORE_7_OFFSET);
            break;
        default:
            return;
    }

    *bootAddrRegAddr = _generate_trampoline(hart_id, function, args, stack_ptr);

    // Check if the cluster is busy
    wait_pulpCluster_busy(cluster_id);
    // Enable desired core of given cluster
    *fetchEnRegAddr = 0x01 << core_id;    
}

/**
 * @brief Offload a void function pointer to a cluster.
 * The function will be executed on all cores of the cluster.
 *
 * @param function Function pointer to offload
 * @param args Arguments to pass to the function
 * @param stack_ptr Stack pointer for the core
 * @param cluster_id ID of the cluster to offload to
 */
void offload_pulpCluster(void *function, void *args, void *stack_ptr, uint8_t cluster_id) {

    // Core with hart_id 0 is CVA6's, thus we start with 1
    uint32_t hart_id = 1;
    for (uint32_t i = 0; i < cluster_id; i++) {
        hart_id += _chimera_numCores[i];
    }

    // Address of PULP Cluster's boot address register
    volatile void **clusterBaseAddr;
    volatile void **bootAddrRegAddrs[8];
    // Address of PULP Cluster's fetch-enable register
    volatile uint32_t *fetchEnRegAddr;

    // Compute cluster offset
    switch (cluster_id) {
        case 0:
            clusterBaseAddr = (volatile void **)(CLUSTER_0_BASE);
            break;
        case 1:
            clusterBaseAddr = (volatile void **)(CLUSTER_1_BASE);
            break;
        case 2:
            clusterBaseAddr = (volatile void **)(CLUSTER_2_BASE);
            break;
        case 3:
            clusterBaseAddr = (volatile void **)(CLUSTER_3_BASE);
            break;
        case 4:
            clusterBaseAddr = (volatile void **)(CLUSTER_4_BASE);
            break;
        default:
            return;
    }

    bootAddrRegAddrs[0] = (volatile void **)((uint8_t *)clusterBaseAddr + BOOT_ADDR_CORE_0_OFFSET);
    bootAddrRegAddrs[1] = (volatile void **)((uint8_t *)clusterBaseAddr + BOOT_ADDR_CORE_1_OFFSET);
    bootAddrRegAddrs[2] = (volatile void **)((uint8_t *)clusterBaseAddr + BOOT_ADDR_CORE_2_OFFSET);
    bootAddrRegAddrs[3] = (volatile void **)((uint8_t *)clusterBaseAddr + BOOT_ADDR_CORE_3_OFFSET);
    bootAddrRegAddrs[4] = (volatile void **)((uint8_t *)clusterBaseAddr + BOOT_ADDR_CORE_4_OFFSET);
    bootAddrRegAddrs[5] = (volatile void **)((uint8_t *)clusterBaseAddr + BOOT_ADDR_CORE_5_OFFSET);
    bootAddrRegAddrs[6] = (volatile void **)((uint8_t *)clusterBaseAddr + BOOT_ADDR_CORE_6_OFFSET);
    bootAddrRegAddrs[7] = (volatile void **)((uint8_t *)clusterBaseAddr + BOOT_ADDR_CORE_7_OFFSET);

    for (uint32_t i = 0; i < 8; i++) {
        *(bootAddrRegAddrs[i]) = _generate_trampoline(hart_id, function, args, stack_ptr);
    }

    // Check if the cluster is busy
    wait_pulpCluster_busy(cluster_id);

    // Enable all cores of the given cluster
    fetchEnRegAddr = (volatile uint32_t *)(clusterBaseAddr + FETCH_EN_OFFSET);
    *fetchEnRegAddr = 0x00FF;
}

/**
 * @brief Blocking wait for the cluster to become idle.
 * The function busy waits until the cluster is ready.
 *
 * @warning In the current PULP Cluster trampoline function implementation, the core 0 of each
 * cluster clears the busy flag as soon as it returns (only core 0 drives the busy flag). Hence
 * the busy flag does not reflect the real status of the cluster.
 *
 * @todo Fix the busy mechanism after adding synchornization primitives for the PULP cores.
 *
 * @param cluster_id ID of the cluster to wait for.
 */
void wait_pulpCluster_busy(uint8_t cluster_id) {
    volatile uint32_t *busy_ptr;

    if (cluster_id == 0) {
        busy_ptr = (volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_BUSY_REG_OFFSET);
    } else if (cluster_id == 1) {
        busy_ptr = (volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_1_BUSY_REG_OFFSET);
    } else if (cluster_id == 2) {
        busy_ptr = (volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_2_BUSY_REG_OFFSET);
    } else if (cluster_id == 3) {
        busy_ptr = (volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_3_BUSY_REG_OFFSET);
    } else if (cluster_id == 4) {
        busy_ptr = (volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_4_BUSY_REG_OFFSET);
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
 * @param cluster_id ID of the cluster to wait for.
 * @return uint32_t Return value of the cluster.
 */
uint32_t wait_pulpCluster_return(uint8_t cluster_id) {
    volatile int32_t *returnAddr;
    if (cluster_id == 0) {
        returnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_RETURN_REG_OFFSET);
    } else if (cluster_id == 1) {
        returnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_1_RETURN_REG_OFFSET);
    } else if (cluster_id == 2) {
        returnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_2_RETURN_REG_OFFSET);
    } else if (cluster_id == 3) {
        returnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_3_RETURN_REG_OFFSET);
    } else if (cluster_id == 4) {
        returnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_4_RETURN_REG_OFFSET);
    }

    while (*returnAddr == 0) {
    }

    uint32_t retVal = *returnAddr;
    *returnAddr = 0;

    return retVal;
}

/** @}*/
