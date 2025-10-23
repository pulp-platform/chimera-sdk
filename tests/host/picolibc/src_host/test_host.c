// Copyright 2025 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Philip Wiese <wiesep@iis.ee.ethz.ch>

// Include Standard Libraries
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

// Include Application Headers

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers

// Import HAL Headers

int main(void) {
    // memcpy
    volatile int32_t src[4] = {1, 2, 3, 4}; // Use volatile to prevent optimization
    volatile int32_t dst[4] = {0, 0, 0, 0};
    memcpy((void *)dst, (const void *)src, sizeof(src));

    // Math (Integer)
    volatile int32_t intval = 42;   // Use volatile to prevent optimization
    intval = __builtin_clz(intval); // Use volatile to prevent optimization
    intval = __builtin_ctz(intval); // Use volatile to prevent optimization

    // Math (Float)
    volatile float testval = 0.5f;                           // Use volatile to prevent optimization
    testval = sinf(testval) + cosf(testval) + tanf(testval); // Use volatile to prevent optimization
    testval = logf(testval) + expf(testval);                 // Use volatile to prevent optimization

    return 0;
}
