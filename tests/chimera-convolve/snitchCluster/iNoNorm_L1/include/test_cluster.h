// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _TEST_CLUSTER_INCLUDE_GUARD_
#define _TEST_CLUSTER_INCLUDE_GUARD_

#include <stdint.h>

void clusterInterruptHandler();

int32_t inonorm_l1_test(void *args);

extern int8_t weight_tensor[16384];
extern int32_t bias_tensor[16384];

#endif //_TEST_CLUSTER_INCLUDE_GUARD_
