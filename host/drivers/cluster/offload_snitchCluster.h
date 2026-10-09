// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * @defgroup drivers Drivers
 * @brief Low-level peripheral and device drivers for Chimera-SDK.
 */

/**
 * \defgroup drivers_snitch_cluster Snitch Cluster Device Driver
 * @ingroup drivers
 * @brief Driver for offloading tasks to Snitch clusters in Chimera-SDK.
 * @{
 */

// Interrupts
/**
 * @brief Setup the interrupt handler for the cluster cores.
 * All cores in all clusters will jump to the handler when an interrupt is triggered.
 *
 * @param handler Function pointer to the interrupt handler
 */
void setup_snitchCluster_interruptHandler(void *handler);

/**
 * @brief Generate the stack pointers for all cores of a given cluster using given stack sizes
 *    passed on a per-core basis. The stack pointers are generated compliant to the RISC-V ABI,
 *    are aligned to 16-Byte boundaries and are assumed to grow downwards.
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
                                 void **stack_ptr);

/**
 * @brief Generate the stack pointers for all cores of a given cluster using the given stack size
 *    equal for all cores. The stack pointers are generated compliant to the RISC-V ABI,
 *    are aligned to 16-Byte boundaries and are assumed to grow downwards.
 *
 * @param clusterId ID of the cluster to generate the stack pointers for
 * @param sp The memory address that serves as a base for the stack pointer
 * @param stack_size The stack size per core in the cluster in Bytes.
 * @param[out] stack_ptr An array to hold the generated stack pointers.
 *    Must reference an array whose length matches the number of cores in the specified cluster.
 *
 * @returns The pointer to the end of available memory after allocating all stacks.
 */
void *generate_snitchCluster_SPs_uniform(uint8_t clusterId, void *sp, uint32_t stack_size,
                                         void **stack_ptr);

// Function Offloading
/**
 * @brief Offload a void function pointer to a cluster.
 * The function will be executed on all cores of the cluster. Waits for the cluster to become
 * idle before offloading.
 *
 * @param function Function pointer to offload
 * @param trampoline Address of the trampoline in the device binary. The trampoline loads the
 *    stack pointer, function pointer and argument from the shared data and calls the function.
 * @param args Arguments to pass to the function
 * @param stack_ptr Array containing the stack pointers for the cores. The length must equal the
 *    number of cores in the cluster
 * @param clusterId ID of the cluster to offload to
 */
void offload_snitchCluster(void *function, void *trampoline, void *args, void **stack_ptr,
                           uint8_t clusterId);

/**
 * @brief Offload a void function pointer to a cluster's core.
 * The function will be executed on the specified core of the cluster.
 *
 * @param function Function pointer to offload
 * @param trampoline Address of the trampoline in the device binary. The trampoline loads the
 *    stack pointer, function pointer and argument from the shared data and calls the function.
 * @param args Arguments to pass to the function
 * @param stack_ptr Stack pointer for the core
 * @param clusterId ID of the cluster to offload to
 * @param core_id ID of the core to offload to (cores are 0-indexed for each cluster)
 */
void offload_snitchCluster_core(void *function, void *trampoline, void *args, void *stack_ptr,
                                uint8_t clusterId, uint32_t core_id);

/**
 * @brief Set Soft Reset on specified cluster
 * @param clusterId ID of the cluster to set soft reset for
 * @param enable true to enable soft reset, false to disable
 */
void set_snitchCluster_reset(uint8_t clusterId, bool enable);

/**
 * @brief Set Soft Reset on all clusters
 * @param enable true to enable soft reset, false to disable
 */
void setAll_snitchCluster_reset(bool enable);

/**
 * @brief Set Clock Gating on specified cluster
 * @param clusterId ID of the cluster to set clock gating for
 * @param enable true to enable clock gating, false to disable
 */
void set_snitchCluster_clockGating(uint8_t clusterId, bool enable);

/**
 * @brief Set Clock Gating on all clusters
 * @param enable true to enable clock gating, false to disable
 */
void setAll_snitchCluster_clockGating(bool enable);

// Synchronization
/**
 * @brief Check if the cluster is busy.
 *
 * @param clusterId ID of the cluster to check
 * @return int Return 1 if the cluster is busy, 0 if it is idle, -1 if the cluster ID is invalid
 */
int snitchCluster_busy(uint8_t clusterId);

/**
 * @brief Blocking wait for the cluster to become idle.
 * The function busy waits until the cluster is ready.
 *
 * @warning The busy flag is set by the offloaded function itself (`_SET_CLUSTER_BUSY()`), not by
 * hardware. Hence the busy flag does not reflect the actual status of the cluster right after an
 * offload. A fixed NOP delay after the flag clears is used as a temporary workaround.
 *
 * @todo Replace the delay after adding synchronization primitives for the Snitch cores.
 *
 * @param clusterId ID of the cluster to wait for.
 */
void wait_snitchCluster_busy(uint8_t clusterId);

/**
 * @brief Wait for the cluster to return a value. The return value is written
 * by the last core of the cluster.
 * The function busy waits until the cluster returns a non-zero value.
 *
 * @warning The return values must be non-zero, otherwise the function will busy wait forever!
 *
 * @param clusterId ID of the cluster to wait for.
 * @return uint32_t Return value of the cluster.
 */
uint32_t wait_snitchCluster_return(uint8_t clusterId);

/** @} */
