// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _TEST_HOST_INCLUDE_GUARD_
#define _TEST_HOST_INCLUDE_GUARD_

#include <stdint.h>
#include <stddef.h>

typedef enum direction { DMA_READ_L2 = 0, DMA_WRITE_L2 = 1 } dma_direction_t;

typedef struct {
    dma_direction_t direction;
    void *pointer_l2;
    size_t size_bytes;
} dma_l2_test_args_t;

#endif //_TEST_HOST_INCLUDE_GUARD_
