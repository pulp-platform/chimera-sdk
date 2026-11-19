// SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#include <stdint.h>
#include <stdio.h>

#include "test_host.h"

#include "test_deviceTypes_bare_symbols.h"
#include "test_deviceTypes_snitch_symbols.h"

#include "driver.h"
#include "soc.h"

// Both binaries run on this cluster, one after the other
#define CLUSTER 0

static offloadArgs_t offloadArgs = {.value = 0xdeadbeef};

static uint32_t run(void *handler, void *function, void *trampoline) {
    void *stack_ptr[NUM_CLUSTER_CORES];
    void *stack = (void *)(_chimera_clusterBase[CLUSTER] + 0x20000 - 1);

    generate_snitchCluster_SPs_uniform(CLUSTER, stack, 0x2000, stack_ptr);
    setup_snitchCluster_interruptHandler(handler);
    set_snitchCluster_clockGating(CLUSTER, 0);
    set_snitchCluster_reset(CLUSTER, 1);
    for (volatile int i = 0; i < 10; i++);
    set_snitchCluster_reset(CLUSTER, 0);

    offload_snitchCluster(function, trampoline, &offloadArgs, stack_ptr, CLUSTER);
    uint32_t ret = wait_snitchCluster_return(CLUSTER);
    set_snitchCluster_clockGating(CLUSTER, 1);
    return ret;
}

int main(void) {
    uint32_t snitch = run(snitch_clusterInterruptHandler, snitch_testReturn, snitch_trampoline);
    uint32_t bare = run(bare_clusterInterruptHandler, bare_testReturn, bare_trampoline);

    printf("snitch_cluster returned 0x%08x, expected 0x%08x\n", snitch, TESTVAL_SNITCH | 1);
    printf("test_bare returned 0x%08x, expected 0x%08x\n", bare, TESTVAL_BARE | 1);
    return (snitch != (TESTVAL_SNITCH | 1)) || (bare != (TESTVAL_BARE | 1));
}
