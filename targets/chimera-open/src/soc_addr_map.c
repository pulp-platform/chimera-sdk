// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Single definition of all address arrays declared in soc_addr_map.h.
// Keeping the definitions here (not in the header) avoids a static copy per
// translation unit that includes the header.

#include "addr_maps/soc_addr_map.h"

const uintptr_t _chimera_clusterBase[_chimera_numClusters] = {
    CLUSTER_0_BASE, CLUSTER_1_BASE, CLUSTER_2_BASE, CLUSTER_3_BASE, CLUSTER_4_BASE};

const uintptr_t _chimera_clusterHeapStart[_chimera_numClusters] = {
    (uintptr_t)__l1_c0_heap_start, (uintptr_t)__l1_c1_heap_start, (uintptr_t)__l1_c2_heap_start,
    (uintptr_t)__l1_c3_heap_start, (uintptr_t)__l1_c4_heap_start};

const uintptr_t _chimera_clusterL1Start[_chimera_numClusters] = {
    (uintptr_t)__l1_c0_start, (uintptr_t)__l1_c1_start, (uintptr_t)__l1_c2_start,
    (uintptr_t)__l1_c3_start, (uintptr_t)__l1_c4_start};

const uintptr_t _chimera_clusterL1End[_chimera_numClusters] = {
    (uintptr_t)__l1_c0_end, (uintptr_t)__l1_c1_end, (uintptr_t)__l1_c2_end, (uintptr_t)__l1_c3_end,
    (uintptr_t)__l1_c4_end};

const uintptr_t _chimera_clusterL1LmaStart[_chimera_numClusters] = {
    (uintptr_t)__l1_c0_lma_start, (uintptr_t)__l1_c1_lma_start, (uintptr_t)__l1_c2_lma_start,
    (uintptr_t)__l1_c3_lma_start, (uintptr_t)__l1_c4_lma_start};

const uintptr_t _chimera_clusterL1LmaEnd[_chimera_numClusters] = {
    (uintptr_t)__l1_c0_lma_end, (uintptr_t)__l1_c1_lma_end, (uintptr_t)__l1_c2_lma_end,
    (uintptr_t)__l1_c3_lma_end, (uintptr_t)__l1_c4_lma_end};
