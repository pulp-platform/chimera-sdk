// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _ITA_MEM_INCLUDE_GUARD_
#define _ITA_MEM_INCLUDE_GUARD_

#include <stdint.h>

#include "ITA_types.h"
#include "ITA_dims.h"

// clang-format off
extern const int8_t input[4232];
extern const int8_t kernel[32768];
extern const int8_t golden[16384];
extern const uint32_t requant_eps_mult[1][2];
extern const uint32_t requant_right_shift[1][2];
extern const int32_t requant_add[1][2];

// clang-format on

#endif
