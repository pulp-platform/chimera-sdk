// SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

#include "addr_maps/soc_addr_map.h"

// Static, since the soc_addr_map.h cluster tables have internal linkage
static inline uint32_t __attribute__((const)) snrt_platform_cluster_num(void) {
    return _chimera_numClusters;
}

static inline uint32_t __attribute__((const)) snrt_platform_cluster_core_num(uint32_t cluster_idx) {
    return _chimera_numCores[cluster_idx];
}

static inline uint32_t __attribute__((const))
snrt_platform_cluster_base_hartid(uint32_t cluster_idx) {
    return _chimera_hartBase[cluster_idx];
}
