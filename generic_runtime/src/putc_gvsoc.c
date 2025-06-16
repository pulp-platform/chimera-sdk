// Copyright 2025 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Victor Jung <jungvi@iis.ee.ethz.ch>

#include <stdio.h>
#include <stdint.h>

#include "uart_setup.h"

char uart_putc(char c, FILE *file) {
    (void)file;
    *(volatile uint32_t *)(long)(0x03004000) = c;
    return c;
}
