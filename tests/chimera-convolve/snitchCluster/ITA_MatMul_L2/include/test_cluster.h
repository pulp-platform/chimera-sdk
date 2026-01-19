// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _TEST_CLUSTER_INCLUDE_GUARD_
#define _TEST_CLUSTER_INCLUDE_GUARD_

#include <stdint.h>

// #include "ITA_dims_64x64x64.h"
// #include "ITA_dims_64x128x64.h"
#include "ITA_dims_64x256x64.h"
// #include "ITA_dims_64x512x64.h"
// #include "ITA_dims_128x512x64.h"
// #include "ITA_dims_192x512x64.h"
// #include "ITA_dims_256x512x64.h"
// #include "ITA_dims_512x512x64.h"

void clusterInterruptHandler();

int32_t ita_matmul_l2_test(void *args);

#endif //_TEST_CLUSTER_INCLUDE_GUARD_
