// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _TEST_HOST_INCLUDE_GUARD_
#define _TEST_HOST_INCLUDE_GUARD_

#include <stdint.h>

typedef struct __attribute__((packed)) {
    volatile uint32_t errors;
    volatile uint32_t runtime_cycles;
    volatile uint32_t ops_per_cycle; // Multiplied by 1e3
    volatile uint32_t reserved[4];
} argCluster_t;

#endif //_TEST_HOST_INCLUDE_GUARD_
