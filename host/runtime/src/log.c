// Copyright 2025 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Philip Wiese <wiesep@iis.ee.ethz.ch>

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