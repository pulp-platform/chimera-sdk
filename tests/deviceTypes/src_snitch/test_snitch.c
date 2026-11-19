// SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#include "test_cluster.h"
#include "test_host.h"

#include "snrt.h"

#if !defined(CHIMERA_DEVICE_SNITCH_CLUSTER) || defined(CHIMERA_DEVICE_TEST_BARE)
#error "A snitch_cluster binary must get the snitch_cluster runtime only"
#endif

int32_t testReturn(void *args) {
    return ((offloadArgs_t *)args)->value == 0xdeadbeef ? TESTVAL_SNITCH : -1;
}
