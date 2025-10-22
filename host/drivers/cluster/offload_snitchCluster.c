// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Moritz Scherer <scheremo@iis.ee.ethz.ch>
// Philip Wiese <wiesep@iis.ee.ethz.ch>
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

// Include Standard Libraries
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "offload_snitchCluster.h"

// Include Runtime Headers
#include "util.h"

// Import HAL Headers
#include "device_api.h"

/**
 * \defgroup drivers_snitch_cluster Snitch Cluster Device Driver
 * @ingroup drivers
 * @brief Driver for offloading tasks to Snitch clusters in Chimera-SDK.
 * @{
 */

/**
 * @brief Persistent trampoline function pointers for each cluster core.
 *
 * Each entry holds the function to be called by the trampoline on the corresponding core.
 */
void (*_trampoline_function[NUM_CLUSTER_CORES])(void *) = {NULL};

/**
 * @brief Persistent argument storage for each cluster core's trampoline function.
 *
 * Each entry holds the `void*` argument passed to the trampoline on the corresponding core.
 */
void *_trampoline_args[NUM_CLUSTER_CORES] = {NULL};

/**
 * @brief Persistent stack pointer storage for each cluster core's trampoline context.
 *
 * Each entry holds the stack pointer to be loaded by the trampoline on the corresponding core.
 */
void *_trampoline_stack[NUM_CLUSTER_CORES] = {NULL};

/// @cond DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Trampoline function for the cluster core.
 * This function will set up the stack pointer and call the function.
 *
 * @warning Make sure that this function is compiled with ISA for the Snitch cores (RV32IM)
 *
 */
extern void _trampoline();
/// @endcond

/**
 * @brief Generate a trampoline function for the cluster core.
 * The trampoline function will set up the stack pointer and call the function.
 *
 * @param hartID hart ID of the core to offload to
 * @param function Function pointer to offload
 * @param args Arguments to pass to the function
 * @param stack Stack pointer for core
 * @return A pointer to the persistent trampoline function
 */
static void *_generate_trampoline(uint32_t hartID, void (*function)(void *), void *args,
                                  void *stack) {
    uint32_t trampoline_idx = hartID - CLUSTER_HART_BASE;

    // Assign trampoline with captured arguments to the persistent function pointer
    _trampoline_function[trampoline_idx] = function;
    _trampoline_args[trampoline_idx] = args;
    _trampoline_stack[trampoline_idx] = stack;

// Store captured arguments in a persistent context if needed
#ifdef TRACE
    printf("  Function  : %p @ %p\n", function, &_trampoline_function[trampoline_idx]);
    printf("  Args      : %p @ %p\n", args, &_trampoline_args[trampoline_idx]);
    printf("  Stack     : %p @ %p\n", stack, &_trampoline_stack[trampoline_idx]);
    printf("  Trampoline: %p\n", _trampoline);
#endif
    return _trampoline;
}

/**
 * @brief Get the hart ID of a core within a cluster.
 *
 * @param clusterId ID of the cluster
 * @param core_id ID of the core
 *
 * @return uint32_t Hart ID of the core
 */
static uint32_t _get_hart_id(uint32_t clusterId, uint32_t core_id) {
    return _chimera_hartBase[clusterId] + core_id;
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
 * @brief Generate the stack pointers for all cores a given cluster using given stack sizes
 *    passed on a per-core bases. The stack pointers are generated compliant to the RISCV-ABI
 *    and are aligned to 16-Byte boundaries and assumed to grow downwards.
 *
 * @param clusterId ID of the cluster to generate the stack pointers for
 * @param sp The memory address that serves as a base for the stack pointer
 * @param[in] stack_size An array containing the requested stack sizes in Bytes.
 *    Must reference an array whose length matches the number of cores in the specified cluster.
 * @param[out] stack_ptr An array to hold the generated stack pointers.
 *    Must reference an array whose length matches the number of cores in the specified cluster.
 *
 * @returns The pointer to the end of available memory after allocating all stacks.
 */
void *generate_snitchCluster_SPs(uint8_t clusterId, void *sp, uint32_t *stack_size,
                                 void **stack_ptr) {
    for (uint32_t core_id = 0; core_id < _chimera_numCores[clusterId]; core_id++) {
        // Align to 16 Byte boundaries
        sp = (void *)((uintptr_t)sp & ~(uintptr_t)0xFUL);
        // set stack pointer
        stack_ptr[core_id] = sp;
        // subtract the required amount of stack
        sp -= stack_size[core_id];
    }
    return sp;
}

/**
 * @brief Generate the stack pointers for all cores a given cluster using the given stack size
 *    equal for all cores. The stack pointers are generated compliant to the RISCV-ABI
 *    and are aligned to 16-Byte boundaries and assumed to grow downwards.
 *
 * @param clusterId ID of the cluster to generate the stack pointers for
 * @param sp The memory address that serves as a base for the stack pointer
 * @param stack_size The stack size per core in the cluster in bytes.
 * @param[out] stack_ptr An array to hold the generated stack pointers.
 *    Must reference an array whose length matches the number of cores in the specified cluster.
 *
 * @returns The pointer to the end of available memory after allocating all stacks.
 */
void *generate_snitchCluster_SPs_uniform(uint8_t clusterId, void *sp, uint32_t stack_size,
                                         void **stack_ptr) {
    for (uint32_t core_id = 0; core_id < _chimera_numCores[clusterId]; core_id++) {
        // Align to 16 Byte boundaries
        sp = (void *)((uintptr_t)sp & ~(uintptr_t)0xFUL);
#ifdef TRACE
        printf("Cluster %d Core %d stack @ %p with size: %d \n", clusterId, core_id, sp,
               stack_size);
#endif
        // set stack pointer
        stack_ptr[core_id] = sp;
        // subtract the required amount of stack
        sp -= stack_size;
    }
    return sp;
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

    uint32_t hartId = _get_hart_id(clusterId, core_id);

    printf("Offloading to core %d in cluster %d with hartid %d\n", core_id, clusterId, hartId);

    *snitchBootAddr = _generate_trampoline(hartId, function, args, stack_ptr);

    // Check if the cluster is busy
    wait_snitchCluster_busy(clusterId);
    volatile uint32_t *interruptTarget = ((uint32_t *)CLINT_CTRL_BASE) + hartId;
    *interruptTarget = 1;
}

/**
 * @brief Offload a void function pointer to a cluster.
 * The function will be executed on all cores of the cluster.
 *
 * @param function Function pointer to offload
 * @param args Arguments to pass to the function
 * @param stack_ptr Array containing the stack pointers for the cores. The length must equal the
 * number of cores in the cluster
 * @param clusterId ID of the cluster to offload to
 */
void offload_snitchCluster(void *function, void *args, void **stack_ptr, uint8_t clusterId) {
    volatile void **snitchBootAddr =
        (volatile void **)(SOC_CTRL_BASE + CHIMERA_SNITCH_BOOT_ADDR_REG_OFFSET);

    uint32_t hartId = _get_hart_id(clusterId, 0);

#ifdef TRACE
    printf("Offloading to all cores in cluster %d starting at hartid %d\n", clusterId, hartId);
#endif

    // Check if the cluster is busy
    wait_snitchCluster_busy(clusterId);

    for (uint32_t i = 0; i < _chimera_numCores[clusterId]; i++) {
        *snitchBootAddr = _generate_trampoline(hartId + i, function, args, stack_ptr[i]);
        // Send interrupt to the core
        volatile uint32_t *interruptTarget = ((uint32_t *)CLINT_CTRL_BASE) + hartId + i;
        *interruptTarget = 1;
    }

    // Prevent race contidtion by waiting for all cores to clear the busy flag
    for (volatile int i = 0; i < 1000; i++);
}

/**
 * @brief Check if the cluster is busy.
 *
 * @param clusterId ID of the cluster to check
 * @return int Return 1 if the cluster is busy, 0 if it is idle, -1 if the cluster ID is invalid
 */
int snitchCluster_busy(uint8_t clusterId) {
    volatile int32_t *busy_ptr;

    switch (clusterId) {
    case 0:
        busy_ptr = (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_BUSY_REG_OFFSET);
        break;
    case 1:
        busy_ptr = (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_1_BUSY_REG_OFFSET);
        break;
    case 2:
        busy_ptr = (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_2_BUSY_REG_OFFSET);
        break;
    case 3:
        busy_ptr = (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_3_BUSY_REG_OFFSET);
        break;
    case 4:
        busy_ptr = (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_4_BUSY_REG_OFFSET);
        break;
    default:
        return -1;
    }

    return *busy_ptr;
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
    while (snitchCluster_busy(clusterId) == 1);
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
    switch (clusterId) {
    case 0:
        snitchReturnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CLUSTER_0_RETURN_REG_OFFSET);
        break;
    case 1:
        snitchReturnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CLUSTER_1_RETURN_REG_OFFSET);
        break;
    case 2:
        snitchReturnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CLUSTER_2_RETURN_REG_OFFSET);
        break;
    case 3:
        snitchReturnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CLUSTER_3_RETURN_REG_OFFSET);
        break;
    case 4:
        snitchReturnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CLUSTER_4_RETURN_REG_OFFSET);
        break;
    }

    while (*snitchReturnAddr == 0) {
    }

    uint32_t retVal = *snitchReturnAddr;
    *snitchReturnAddr = 0;
    return retVal;
}

/**
 * @brief Set Clock Gating on specified cluster
 * @param clusterId ID of the cluster to set clock gating for
 * @param enable true to enable clock gating, false to disable
 *
 */
void set_snitchCluster_clockGating(uint8_t clusterId, bool enable) {

    switch (clusterId) {
    case 0:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_CLK_GATE_EN_REG_OFFSET) = enable;
        break;
    case 1:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_1_CLK_GATE_EN_REG_OFFSET) = enable;
        break;
    case 2:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_2_CLK_GATE_EN_REG_OFFSET) = enable;
        break;
    case 3:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_3_CLK_GATE_EN_REG_OFFSET) = enable;
        break;
    case 4:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_4_CLK_GATE_EN_REG_OFFSET) = enable;
        break;
    default:
        break;
    }
}

/**
 * @brief Set Clock Gating on all clusters
 * @param enable true to enable clock gating, false to disable
 */
void setAll_snitchCluster_clockGating(bool enable) {
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_CLK_GATE_EN_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_1_CLK_GATE_EN_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_2_CLK_GATE_EN_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_3_CLK_GATE_EN_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_4_CLK_GATE_EN_REG_OFFSET) = enable;
}

/**
 * @brief Set Soft Reset on specified cluster
 * @param clusterId ID of the cluster to set soft reset for
 * @param enable true to enable soft reset, false to disable
 */
void set_snitchCluster_reset(uint8_t clusterId, bool enable) {
    switch (clusterId) {
    case 0:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_0_REG_OFFSET) = enable;
        break;
    case 1:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_1_REG_OFFSET) = enable;
        break;
    case 2:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_2_REG_OFFSET) = enable;
        break;
    case 3:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_3_REG_OFFSET) = enable;
        break;
    case 4:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_4_REG_OFFSET) = enable;
        break;
    default:
        break;
    }
}

/**
 * @brief Set Soft Reset on all clusters
 * @param enable true to enable soft reset, false to disable
 */
void setAll_snitchCluster_reset(bool enable) {
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_0_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_1_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_2_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_3_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_4_REG_OFFSET) = enable;
}

static int snitchcluster_open(chi_device_t *dev) {
    (void)dev;
    return 0;
}
static int snitchcluster_close(chi_device_t *dev) {
    (void)dev;
    return 0;
}
static ssize_t snitchcluster_read(chi_device_t *dev, void *buf, uint32_t len,
                                  chi_device_callback_t cb) {
    (void)dev;
    (void)buf;
    (void)len;
    (void)cb;
    return -1;
}

// VIVIANEP: Need to skip doxygen generation for these functions
// to avoid duplicated defintion errors in the generated documentation
/// @cond DOXYGEN_SHOULD_SKIP_THIS
// const chi_device_api_t default_snitchcluster_api = {
//     .open = snitchcluster_open,
//     .close = snitchcluster_close,
//     .read = snitchcluster_read
//     .write = (ssize_t (*)(chi_device_t *, const void *, uint32_t,
//                           chi_device_callback_t))offload_snitchCluster
// };
/// @endcond

/** @} */ // End of drivers_snitch_cluster group
