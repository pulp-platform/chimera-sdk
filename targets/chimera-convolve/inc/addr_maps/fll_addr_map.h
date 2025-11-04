// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _FLL_ADDR_MAP_INCLUDE_GUARD_
#define _FLL_ADDR_MAP_INCLUDE_GUARD_

#include <stdint.h>
#include "soc_addr_map.h"

// FLLs registers
#ifndef FLL_BASE_ADDR
#error "FLL_BASE_ADDR is not defined. It must be declared in the soc_addr_map.h"
#endif
#define FLL_SOC_BASE FLL_BASE_ADDR
#define FLL_CLU_BASE FLL_BASE_ADDR + 16

#define FLL_STATUS_OFFSET 0x0
#define FLL_CONF1_OFFSET 0x4
#define FLL_CONF2_OFFSET 0x8
#define FLL_INTEGRATOR_OFFSET 0xc

#define FLL_MULT_BIT 0
#define FLL_MULT_WIDTH 16
#define FLL_MULT_MASK 0xFFFF

#endif //_FLL_ADDR_MAP_INCLUDE_GUARD_
