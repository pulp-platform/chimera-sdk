// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potcnik <vivianep@iis.ee.ethz.ch>

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdlib.h>
#include "device_api.h"

#define DUMP(val) ({ asm volatile("csrw 0x7FF, %0" ::"rK"(val)); })

// Parity options
#define UART_PARITY_NONE 0
#define UART_PARITY_EVEN 1
#define UART_PARITY_ODD 2

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

typedef struct uart_context {
    void *driver_context;
    void *driver_api;
} uart_context_t;

// Function declarations
extern int uart_open(struct chi_device *device);
extern int uart_close(struct chi_device *device);
extern ssize_t uart_read(struct chi_device *device, void *buffer, uint32_t size,
                         chi_device_callback cb);
extern ssize_t uart_write(struct chi_device *device, const void *buffer, uint32_t size,
                          chi_device_callback cb);

// Extern the UART API structure
extern struct chi_device_api uart_api;
extern uart_config_t default_cfg;

#endif // UART_H