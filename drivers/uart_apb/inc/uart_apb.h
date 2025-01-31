// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#ifndef UART_APB_H
#define UART_APB_H

#include "uart.h"
#include "util.h"

// Register offsets
#define UART_RBR_REG_OFFSET 0
#define UART_THR_REG_OFFSET 0
#define UART_INTR_ENABLE_REG_OFFSET 4
#define UART_INTR_IDENT_REG_OFFSET 8
#define UART_FIFO_CONTROL_REG_OFFSET 8
#define UART_LINE_CONTROL_REG_OFFSET 12
#define UART_MODEM_CONTROL_REG_OFFSET 16
#define UART_LINE_STATUS_REG_OFFSET 20
#define UART_MODEM_STATUS_REG_OFFSET 24
#define UART_DLAB_LSB_REG_OFFSET 0
#define UART_DLAB_MSB_REG_OFFSET 4

// Register fields
#define UART_LINE_STATUS_DATA_READY_BIT 0
#define UART_LINE_STATUS_THR_EMPTY_BIT 5
#define UART_LINE_STATUS_TMIT_EMPTY_BIT 6

#endif // UART_APB_H