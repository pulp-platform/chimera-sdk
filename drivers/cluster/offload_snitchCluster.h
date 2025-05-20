// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Philip Wiese <wiesep@iis.ee.ethz.ch>

#ifndef _CLUSTER_SNITCH_OFFLOAD_INCLUDE_GUARD_
#define _CLUSTER_SNITCH_OFFLOAD_INCLUDE_GUARD_

#include <stdint.h>

/** \addtogroup cluster
 *  @{
 */

// Interrupts
/**
 * @brief Setup the interrupt handler for the cluster cores.
 * All cores in all clusters will jump to the handler when an interrupt is triggered.
 *
 * @param handler Function pointer to the interrupt handler
 */
void setup_snitchCluster_interruptHandler(void *handler);

// Function Offloading
void *generate_snitchCluster_SPs(uint8_t clusterId, void *sp, uint32_t *stack_size,
                                 void **stack_ptr);
void *generate_snitchCluster_SPs_uniform(uint8_t clusterId, void *sp, uint32_t stack_size,
                                         void **stack_ptr);
void offload_snitchCluster(void *function, void *args, void *stack_ptr, uint8_t clusterId);
void offload_snitchCluster_core(void *function, void *args, void *stack_ptr, uint8_t clusterId,
                                uint32_t core_id);

// Synchronization
void wait_snitchCluster_busy(uint8_t clusterId);
uint32_t wait_snitchCluster_return(uint8_t clusterId);

/** @} */

#endif //_CLUSTER_SNITCH_OFFLOAD_INCLUDE_GUARD_