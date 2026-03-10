// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Define the standard C I/O streams for the Snitch cluster device binary.
// picolibc's printf/vfprintf require stdin/stdout/stderr to be defined.
// We back them with snrt_putchar so that cluster printf output goes via
// the same HTIF semihosting path as snrt_printf.

#include <stdio.h>

// Forward declaration — defined in putc.c
int snrt_putchar(char c, FILE *file);

static FILE __stdio_cluster = FDEV_SETUP_STREAM(snrt_putchar, NULL, NULL, _FDEV_SETUP_WRITE);

FILE *const stdin = &__stdio_cluster;
FILE *const stdout = &__stdio_cluster;
FILE *const stderr = &__stdio_cluster;
