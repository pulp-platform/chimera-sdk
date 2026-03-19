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

void *generate_snitchCluster_SPs(uint8_t clusterId, void *sp, uint32_t *stack_size,
                                 void **stack_ptr);
void *generate_snitchCluster_SPs_uniform(uint8_t clusterId, void *sp, uint32_t stack_size,
                                         void **stack_ptr);

// Function Offloading
void offload_snitchCluster(void *function, void *trampoline, void *args, void **stack_ptr,
                           uint8_t clusterId);
void offload_snitchCluster_core(void *function, void *trampoline, void *args, void *stack_ptr,
                                uint8_t clusterId, uint32_t core_id);

void set_snitchCluster_reset(uint8_t clusterId, bool enable);
void setAll_snitchCluster_reset(bool enable);
void set_snitchCluster_clockGating(uint8_t clusterId, bool enable);
void setAll_snitchCluster_clockGating(bool enable);

// Synchronization
int snitchCluster_busy(uint8_t clusterId);
void wait_snitchCluster_busy(uint8_t clusterId);
uint32_t wait_snitchCluster_return(uint8_t clusterId);

/** @} */ // End of drivers_snitch_cluster group
