// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _ITA_MEM_INCLUDE_GUARD_
#define _ITA_MEM_INCLUDE_GUARD_

#include <stdint.h>

#include "ITA_types.h"
#include "ITA_dims.h"

// clang-format off
extern const int8_t input[INPUT_C*INPUT_H*INPUT_W];
extern const int8_t kernel[KERNEL_C*KERNEL_H*KERNEL_W*INPUT_C];
extern const int8_t golden[OUTPUT_C*OUTPUT_H*OUTPUT_W];
extern const uint32_t requant_eps_mult[1][2];
extern const uint32_t requant_right_shift[1][2];
extern const int32_t requant_add[1][2];

// clang-format on

#endif