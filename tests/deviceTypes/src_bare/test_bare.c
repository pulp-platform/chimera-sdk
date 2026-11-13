// SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#include "test_cluster.h"
#include "test_host.h"

#if defined(CHIMERA_DEVICE_SNITCH_CLUSTER) || !defined(CHIMERA_DEVICE_TEST_BARE)
#error "A test_bare binary must get the test_bare runtime only"
#endif

#if __has_include("snrt.h")
#error "A test_bare binary must not see the snitch_cluster runtime headers"
#endif

int32_t testReturn(void *args) {
    return ((offloadArgs_t *)args)->value == 0xdeadbeef ? TESTVAL_BARE : -1;
}
