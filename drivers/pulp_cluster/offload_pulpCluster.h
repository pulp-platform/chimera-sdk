// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Sergio Mazzola <smazzola@iis.ee.ethz.ch>

#ifndef _CLUSTER_PULP_OFFLOAD_INCLUDE_GUARD_
#define _CLUSTER_PULP_OFFLOAD_INCLUDE_GUARD_

#include <stdint.h>

/** \addtogroup cluster
 *  @{
 */

// Function Offloading
void offload_pulpCluster(void *function, void *args, void *stack_ptr, uint8_t cluster_id);
void offload_pulpCluster_core(void *function, void *args, void *stack_ptr, uint8_t cluster_id,
                              uint32_t core_id);

// Synchronization
void wait_pulpCluster_busy(uint8_t cluster_id);
uint32_t wait_pulpCluster_return(uint8_t cluster_id);

/** @} */

#endif //_CLUSTER_PULP_OFFLOAD_INCLUDE_GUARD_
