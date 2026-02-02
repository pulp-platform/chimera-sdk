// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>
#include <stdarg.h>

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers
#include "log.h"
int printf_log(const char *fmt, ...) {
    int ret;
    va_list args;

    fprintf(stdout, "[HOST ] ");

    va_start(args, fmt);
    ret = vfprintf(stdout, fmt, args);
    va_end(args);

    return ret;
}