// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/*
 * Standard I/O streams for the Snitch cluster (picolibc).
 *
 * picolibc requires stdin/stdout/stderr to be defined as FILE* globals.
 * FDEV_SETUP_STREAM connects them to snrt_putchar (defined in putc.c), which
 * buffers output and flushes via HTIF semihosting.  The read and flush hooks
 * are NULL because cluster cores do not support interactive input and buffering
 * is handled entirely inside snrt_putchar.
 */

#include <stdio.h>

// Forward declaration — defined in putc.c
int snrt_putchar(char c, FILE *file);

static FILE __stdio_cluster = FDEV_SETUP_STREAM(snrt_putchar, NULL, NULL, _FDEV_SETUP_WRITE);

FILE *const stdin = &__stdio_cluster;
FILE *const stdout = &__stdio_cluster;
FILE *const stderr = &__stdio_cluster;
