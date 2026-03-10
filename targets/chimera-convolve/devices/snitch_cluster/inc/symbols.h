// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "addr_maps/soc_addr_map.h"

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
