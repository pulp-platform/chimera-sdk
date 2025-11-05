// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers
#include "uart.h"

static FILE __stdio = FDEV_SETUP_STREAM(uart_putc, uart_getc, uart_flush, _FDEV_SETUP_RW);

FILE *const stdin = &__stdio;
FILE *const stdout = &__stdio;
FILE *const stderr = &__stdio;
