// SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#include "test_cluster.h"
#include "test_host.h"

#include "soc.h"

int32_t testCount(void *args) {
    offloadArgs_t *a = (offloadArgs_t *)args;
    uintptr_t gp, own_gp;
    uint32_t hartid;
    uint32_t cluster = 0;

    // A core that took the other binary's trampoline runs with that binary's global pointer
    asm volatile(".option push\n"
                 ".option norelax\n"
                 "la %0, __global_pointer$\n"
                 ".option pop\n"
                 "mv %1, gp\n"
                 : "=r"(own_gp), "=r"(gp));
    asm volatile("csrr %0, mhartid" : "=r"(hartid));
    while (hartid >= (uint32_t)_chimera_hartBase[cluster] + _chimera_numCores[cluster]) {
        cluster++;
    }
    uint32_t core = hartid - _chimera_hartBase[cluster];
    uint32_t count = a->count[core] + 1;

    a->wrong_gp[core] += (gp != own_gp);
    a->count[core] = count;
    // Cluster hardware barrier (CSR 0x7C2), once the stores have completed
    asm volatile("fence\n"
                 "csrr x0, 0x7C2" ::
                     : "memory");

    // Only the first core's return value reaches the host
    if (core == 0) {
        for (uint32_t i = 0; i < _chimera_numCores[cluster]; i++) {
            if (a->count[i] != count || a->wrong_gp[i] != 0) {
                return TEST_FAILED;
            }
        }
    } else if (!a->last) {
        // The other cores keep running until the next offload's wake-up is pending
        while (((volatile uint32_t *)CLINT_CTRL_BASE)[hartid] == 0);
    }
    return (DEVICE_TAG << 16) | (count << 1);
}
