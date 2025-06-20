// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Victor Jung <jungvi@iis.ee.ethz.ch>

#include "uart.h"

#include <stdio.h>

int main(void) {

    int8_t ret = 0;

    int8_t *bufferA = (int8_t *)memory_island_malloc(42);
    int8_t *bufferB = (int8_t *)memory_island_malloc(58);
    memory_island_free(bufferA);
    int8_t *bufferC = (int8_t *)memory_island_malloc(21);

    if (bufferA != (int8_t *)0x48000008) ret = -1;
    if (bufferB != (int8_t *)0x4800003c) ret = -1;

    if (bufferA != bufferC) ret = -1;

    return ret;
}
