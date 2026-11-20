// SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#include <stdint.h>
#include <stdio.h>

#include "test_host.h"

#include "test_offloadHandshake_a_symbols.h"
#include "test_offloadHandshake_b_symbols.h"

#include "driver.h"
#include "soc.h"

#define NUM_TEST_CLUSTERS 2
#define ROUNDS 4

static offloadArgs_t offloadArgs[NUM_TEST_CLUSTERS];

int main(void) {
    void *stack_ptr[NUM_TEST_CLUSTERS][NUM_CLUSTER_CORES];
    uint32_t errors = 0;

    for (uint8_t c = 0; c < NUM_TEST_CLUSTERS; c++) {
        void *stack = (void *)(_chimera_clusterBase[c] + 0x20000 - 1);
        generate_snitchCluster_SPs_uniform(c, stack, 0x2000, stack_ptr[c]);
        set_snitchCluster_clockGating(c, 0);
        set_snitchCluster_reset(c, 1);
        for (volatile int i = 0; i < 10; i++);
        set_snitchCluster_reset(c, 0);
    }
    setup_snitchCluster_interruptHandler(a_clusterInterruptHandler);

    // Each round starts both binaries back to back, then swaps their clusters without a reset
    for (uint32_t round = 0; round < ROUNDS; round++) {
        uint8_t a_cluster = round & 1;
        uint8_t b_cluster = !a_cluster;

        for (uint8_t c = 0; c < NUM_TEST_CLUSTERS; c++) {
            offloadArgs[c].last = (round == ROUNDS - 1);
        }
        offload_snitchCluster(a_testCount, a_trampoline, &offloadArgs[a_cluster],
                              stack_ptr[a_cluster], a_cluster);
        offload_snitchCluster(b_testCount, b_trampoline, &offloadArgs[b_cluster],
                              stack_ptr[b_cluster], b_cluster);

        for (uint8_t c = 0; c < NUM_TEST_CLUSTERS; c++) {
            uint32_t tag = (c == a_cluster) ? 0xA : 0xB;
            uint32_t expected = (tag << 16) | ((round + 1) << 1) | 1;
            uint32_t ret = wait_snitchCluster_return(c);
            printf("round %d cluster %d returned 0x%08x, expected 0x%08x\n", (int)round, (int)c,
                   ret, expected);
            errors += (ret != expected);
        }
    }

    for (uint8_t c = 0; c < NUM_TEST_CLUSTERS; c++) {
        wait_snitchCluster_busy(c);
        set_snitchCluster_clockGating(c, 1);
    }
    return errors;
}
