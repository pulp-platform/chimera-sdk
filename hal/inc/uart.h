// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potcnik <vivianep@iis.ee.ethz.ch>

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "device_api.h"
#include "sw/device/lib/base/mmio.h"
#include "sw/device/lib/dif/dif_uart.h"
#include "uart_regs.h"
#include <stddef.h>
#include <stdlib.h>

// Parity options
#define UART_PARITY_NONE 0
#define UART_PARITY_EVEN 1
#define UART_PARITY_ODD  2

// Default UART configurations (can be overridden via compiler defines)
#ifndef UART_DEFAULT_BAUD_RATE
#define UART_DEFAULT_BAUD_RATE 115200
#endif

#ifndef UART_DEFAULT_DATA_BITS
#define UART_DEFAULT_DATA_BITS 8
#endif

#ifndef UART_DEFAULT_PARITY
#define UART_DEFAULT_PARITY UART_PARITY_NONE
#endif

#ifndef UART_DEFAULT_STOP_BITS
#define UART_DEFAULT_STOP_BITS 1
#endif

#ifndef UART_CLK_FREQ_HZ
#define UART_CLK_FREQ_HZ 100000000
#endif

#ifndef UART_BASE_ADDR
#define UART_BASE_ADDR 0x40000000
#endif

// UART configuration structure
typedef struct {
    uint32_t baud_rate;
    uint32_t clk_freq_hz;
    uint8_t data_bits;
    uint8_t parity;
    uint8_t stop_bits;
} uart_config_t;

// Function declarations
int uart_open(struct chi_device *device);
int uart_close(struct chi_device *device);
ssize_t uart_read_async(struct chi_device *device, void *buffer, uint32_t size, chi_device_callback cb);
ssize_t uart_write_async(struct chi_device *device, const void *buffer, uint32_t size, chi_device_callback cb);

// Extern the UART API structure
extern struct chi_device_api uart_api;
extern uart_config_t default_cfg;

#endif // UART_H