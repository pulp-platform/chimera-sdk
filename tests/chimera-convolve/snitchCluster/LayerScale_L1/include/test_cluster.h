// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _TEST_CLUSTER_INCLUDE_GUARD_
#define _TEST_CLUSTER_INCLUDE_GUARD_

#include <stdint.h>

void clusterInterruptHandler();

int32_t layerscale_l1_test(void *args);

extern int32_t mul_tensor[384];
extern int32_t add_tensor[384];

#endif //_TEST_CLUSTER_INCLUDE_GUARD_
