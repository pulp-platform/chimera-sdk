/*
 * Copyright 2025 ETH Zurich.
 * Licensed under the Apache License, Version 2.0, see LICENSE for details.
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Victor Jung <jungvi@iis.ee.ethz.ch>
 */

#include <stdio.h>

#include "uart.h"

chi_interface_t uart_iface;

static int uart_putc(char c, FILE *file){
        (void) file;
        uart_write(&uart_iface, &c, 1, NULL);
        return c;
    }

static FILE __stdio = FDEV_SETUP_STREAM(uart_putc,
                        NULL,
                        NULL,
                        _FDEV_SETUP_WRITE);

FILE *const stdin  = &__stdio;
FILE *const stdout = &__stdio;
FILE *const stderr = &__stdio;