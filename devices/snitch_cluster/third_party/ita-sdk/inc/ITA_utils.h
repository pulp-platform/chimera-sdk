// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _ITA_INCLUDE_GUARD_
#define _ITA_INCLUDE_GUARD_

// Include Standard Libraries
#include <stdint.h>

// Include Application Headers

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers

// WIESEP: This part should go into a cluster specific SDK

int check(int step, const uint8_t *data, const uint8_t *golden, int size, const int8_t *l1_buffer,
          int l1_size);

#endif //_ITA_INCLUDE_GUARD_
