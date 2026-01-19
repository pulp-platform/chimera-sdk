// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _TEST_CLUSTER_INCLUDE_GUARD_
#define _TEST_CLUSTER_INCLUDE_GUARD_

#include <stdint.h>

// #include "ITA_dims_S64_E128_P64.h"
#include "ITA_dims_S128_E128_P64.h"
// #include "ITA_dims_S256_E384_P64.h"

void clusterInterruptHandler();

int32_t ita_sha_l2_test(void *args);

#endif //_TEST_CLUSTER_INCLUDE_GUARD_
