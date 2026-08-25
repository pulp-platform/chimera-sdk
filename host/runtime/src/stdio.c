// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/*
 * Standard I/O stream setup for the host (picolibc).
 *
 * picolibc's printf/vfprintf require stdin/stdout/stderr to be defined as
 * FILE* globals.  FDEV_SETUP_STREAM wires them to uart_putc/uart_getc/uart_flush,
 * which are resolved at link time to the backend-appropriate implementations
 * in putc.c.  All three streams share the same backing FILE so that flushing
 * stdout also flushes stderr.
 */

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
