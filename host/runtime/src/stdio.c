// Copyright 2025 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Victor Jung <jungvi@iis.ee.ethz.ch>

// Include Standard Libraries
#include <stdio.h>

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers
#include "uart.h"

/**
 * \defgroup drivers_uart UART Driver
 * @ingroup drivers
 * @ingroup runtime
 * @brief Generic UART driver implementation for Chimera-SDK.
 * @{
 */

static FILE __stdio = FDEV_SETUP_STREAM(uart_putc, NULL, NULL, _FDEV_SETUP_WRITE);

FILE *const stdin = &__stdio;
FILE *const stdout = &__stdio;
FILE *const stderr = &__stdio;

/** @} */ // end defgroup drivers_uart