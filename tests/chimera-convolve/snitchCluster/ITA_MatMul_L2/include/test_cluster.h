// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _TEST_CLUSTER_INCLUDE_GUARD_
#define _TEST_CLUSTER_INCLUDE_GUARD_

#include <stdint.h>

// The matrix shape is selected at configure time via ITA_MATMUL_L2_SHAPE;
// see this test's CMakeLists.txt. The fallback keeps the header self-contained
// for tooling that compiles it outside the CMake build.
#ifndef ITA_DIMS_HEADER
#define ITA_DIMS_HEADER "ITA_dims_64x256x64.h"
#endif

#include ITA_DIMS_HEADER

void clusterInterruptHandler();

int32_t ita_matmul_l2_test(void *args);

#endif //_TEST_CLUSTER_INCLUDE_GUARD_
