// Copyright 2025 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Victor Jung <jungvi@iis.ee.ethz.ch>

#include <stdio.h>

#include "uart.h"
#include "uart_setup.h"

chi_interface_t uart_iface;


char uart_putc(char c, FILE *file){
    (void) file;
    uart_write(&uart_iface, &c, 1, NULL);
    return c;
}
