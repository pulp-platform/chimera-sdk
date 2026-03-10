// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Single definition of all address arrays declared in soc_addr_map.h.
// Keeping the definitions here (not in the header) avoids a static copy per
// translation unit that includes the header.

#include "addr_maps/soc_addr_map.h"

const uintptr_t _chimera_clusterBase[_chimera_numClusters] = {
    CLUSTER_0_BASE, CLUSTER_1_BASE, CLUSTER_2_BASE, CLUSTER_3_BASE, CLUSTER_4_BASE};
