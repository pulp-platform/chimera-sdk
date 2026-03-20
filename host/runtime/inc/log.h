// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#pragma once

// Include Standard Libraries
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @defgroup runtime_log Logging
 * @ingroup runtime
 * @brief Tagged logging utilities for the host core.
 * @{
 */

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers

/**
 * @brief Prints a formatted message to stdout with a @c [HOST ] tag prefix.
 *
 * Behaves like @c printf but prepends "[HOST ] " to every line, making
 * host-side output visually distinct from cluster output in simulation logs.
 *
 * @param fmt  printf-style format string.
 * @param ...  Variadic arguments for the format string.
 * @return     Number of characters written, or a negative value on error.
 */
int printf_log(const char *fmt, ...);

/** @} */
