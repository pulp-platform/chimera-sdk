// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _SOC_ADDR_MAP_INCLUDE_GUARD_
#define _SOC_ADDR_MAP_INCLUDE_GUARD_

#include <stdint.h>

#define CLINT_CTRL_BASE 0x02040000

#define SOC_CTRL_BASE 0x30001000

#define NUM_CLUSTER_CORES 0

#define HOST_NUMCORES 1

#define NUM_CLUSTER_CORES 0
#define NUM_CORES (HOST_NUMCORES + NUM_CLUSTER_CORES)

#define HOST_HART_BASE 0

static const uint8_t _chimera_numCores[] = {0};
static const uint8_t _chimera_hartBase[] = {0};
static const uint32_t _chimera_clusterBase[] = {0};

#define _chimera_numClusters 0

#define CHIMERA_PADFRAME_BASE_ADDRESS 0x30002000
#define FLL_BASE_ADDR 0x30003000

#endif
