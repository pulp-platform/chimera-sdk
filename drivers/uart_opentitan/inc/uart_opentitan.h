// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#ifndef UART_OPENTITAN_H
#define UART_OPENTITAN_H

#include <stdint.h>
#include "uart.h"
#include "sw/device/lib/dif/dif_uart.h"

// Define OpenTitan-specific UART context
typedef struct {
    dif_uart_t uart; // OpenTitan DIF UART handle
} opentitan_uart_context_t;

#endif // UART_OPENTITAN_H