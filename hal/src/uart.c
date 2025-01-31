// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potcnik <vivianep@iis.ee.ethz.ch>

#include "uart.h"

// Define the UART API structure
struct chi_device_api uart_api = {
    .open = uart_open, .close = uart_close, .read_async = uart_read, .write_async = uart_write};

// Default UART configuration
uart_config_t default_cfg = {.baud_rate = UART_DEFAULT_BAUD_RATE,
                             .clk_freq_hz = UART_CLK_FREQ_HZ,
                             .data_bits = UART_DEFAULT_DATA_BITS,
                             .parity = UART_DEFAULT_PARITY,
                             .stop_bits = UART_DEFAULT_STOP_BITS};