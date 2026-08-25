// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _SOC_ADDR_MAP_INCLUDE_GUARD_
#define _SOC_ADDR_MAP_INCLUDE_GUARD_

#include <stdint.h>

#define CLINT_CTRL_BASE 0x02040000

#define SOC_CTRL_BASE 0x30001000
#define PADFRAME_GPIO_BASE 0x30002000
#define PADFRAME_HYPERBUS_BASE 0x30003000
#define FLL_CTRL_BASE_ADDR 0x30004000
#define HYPERBUS_CTRL_BASE 0x30005000

#define CLUSTER_0_BASE 0x40000000
#define CLUSTER_1_BASE 0x40200000
#define CLUSTER_2_BASE 0x40400000
#define CLUSTER_3_BASE 0x40600000
#define CLUSTER_4_BASE 0x40800000

#define HOST_NUMCORES 1
#define CLUSTER_0_NUMCORES 2
#define CLUSTER_1_NUMCORES 2
#define CLUSTER_2_NUMCORES 2
#define CLUSTER_3_NUMCORES 2
#define CLUSTER_4_NUMCORES 9

#define NUM_CLUSTER_CORES \
    (CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES + CLUSTER_2_NUMCORES + CLUSTER_3_NUMCORES + \
     CLUSTER_4_NUMCORES)

#define NUM_CORES (HOST_NUMCORES + NUM_CLUSTER_CORES)

#define HOST_HART_BASE 0
#define CLUSTER_0_HART_BASE (HOST_HART_BASE + HOST_NUMCORES)
#define CLUSTER_1_HART_BASE (CLUSTER_0_HART_BASE + CLUSTER_0_NUMCORES)
#define CLUSTER_2_HART_BASE (CLUSTER_1_HART_BASE + CLUSTER_1_NUMCORES)
#define CLUSTER_3_HART_BASE (CLUSTER_2_HART_BASE + CLUSTER_2_NUMCORES)
#define CLUSTER_4_HART_BASE (CLUSTER_3_HART_BASE + CLUSTER_3_NUMCORES)

#define CLUSTER_HART_BASE CLUSTER_0_HART_BASE

#define _chimera_numClusters 5

static const uint8_t _chimera_numCores[] = {CLUSTER_0_NUMCORES, CLUSTER_1_NUMCORES,
                                            CLUSTER_2_NUMCORES, CLUSTER_3_NUMCORES,
                                            CLUSTER_4_NUMCORES};
static const uint8_t _chimera_hartBase[] = {CLUSTER_0_HART_BASE, CLUSTER_1_HART_BASE,
                                            CLUSTER_2_HART_BASE, CLUSTER_3_HART_BASE,
                                            CLUSTER_4_HART_BASE};

extern const uintptr_t _chimera_clusterBase[_chimera_numClusters];

extern char __l1_c0_heap_start[], __l1_c1_heap_start[], __l1_c2_heap_start[], __l1_c3_heap_start[],
    __l1_c4_heap_start[];
extern const uintptr_t _chimera_clusterHeapStart[_chimera_numClusters];

extern char __l1_c0_start[], __l1_c1_start[], __l1_c2_start[], __l1_c3_start[], __l1_c4_start[];
extern const uintptr_t _chimera_clusterL1Start[_chimera_numClusters];

extern char __l1_c0_end[], __l1_c1_end[], __l1_c2_end[], __l1_c3_end[], __l1_c4_end[];
extern const uintptr_t _chimera_clusterL1End[_chimera_numClusters];

extern char __l1_c0_lma_start[], __l1_c1_lma_start[], __l1_c2_lma_start[], __l1_c3_lma_start[],
    __l1_c4_lma_start[];
extern const uintptr_t _chimera_clusterL1LmaStart[_chimera_numClusters];

extern char __l1_c0_lma_end[], __l1_c1_lma_end[], __l1_c2_lma_end[], __l1_c3_lma_end[],
    __l1_c4_lma_end[];
extern const uintptr_t _chimera_clusterL1LmaEnd[_chimera_numClusters];

#define HYPERBUS_ADDR_START 0x50000000

#endif
