// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _ITA_TYPES_INCLUDE_GUARD_
#define _ITA_TYPES_INCLUDE_GUARD_

// ITA Accelerator
typedef struct __attribute__((__packed__)) {
    int value : 24;
} ita_int24_t;

typedef enum {
    ATTENTION = 0x0,   // 00 in binary
    FEEDFORWARD = 0x1, // 01 in binary
    LINEAR = 0x2       // 10 in binary
} LayerType;

typedef enum {
    IDENTITY = 0x0, // 00 in binary
    GELU = 0x1,     // 01 in binary
    RELU = 0x2      // 10 in binary
} ActivationType;

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#endif //_ITA_TYPES_INCLUDE_GUARD_