// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Moritz Scherer <scheremo@iis.ee.ethz.ch>

#ifndef _SOC_ADDR_MAP_INCLUDE_GUARD_
#define _SOC_ADDR_MAP_INCLUDE_GUARD_

#include <stdint.h>

#define CLINT_CTRL_BASE 0x02040000

#define SOC_CTRL_BASE 0x30001000

#define NUM_CLUSTER_CORES 0

static uint8_t _chimera_numCores[] = {0};

#define _chimera_numClusters 0

#define CHIMERA_PADFRAME_BASE_ADDRESS 0x30002000
#define FLL_BASE_ADDR 0x30003000

#endif
