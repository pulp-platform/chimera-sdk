// SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _TEST_HOST_INCLUDE_GUARD_
#define _TEST_HOST_INCLUDE_GUARD_

#include <stdint.h>

#define MAX_CLUSTER_CORES 16
#define TEST_FAILED 0x7FFE

typedef struct {
    volatile uint32_t count[MAX_CLUSTER_CORES];
    volatile uint32_t wrong_gp[MAX_CLUSTER_CORES];
    volatile uint32_t last;
} offloadArgs_t;

#endif //_TEST_HOST_INCLUDE_GUARD_
