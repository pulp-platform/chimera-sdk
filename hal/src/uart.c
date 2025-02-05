// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#include "uart.h"

__attribute__((weak)) int uart_open(chi_device_t *device) {
    if (!device || !device->device_addr) {
        return -1;
    }
    return 0;
}

__attribute__((weak)) int uart_close(chi_device_t *device) {
    if (!device || !device->device_addr) {
        return -1;
    }
    return 0;
}

__attribute__((weak)) ssize_t uart_read(chi_device_t *device, void *buffer, uint32_t size,
                                        chi_device_callback_t cb) {
    if (!device || !device->device_addr || !buffer || size == 0) {
        return -1;
    }
    return -1;
}

__attribute__((weak)) ssize_t uart_write(chi_device_t *device, const void *buffer, uint32_t size,
                                         chi_device_callback_t cb) {
    if (!device || !device->device_addr || !buffer || size == 0) {
        return -1;
    }
    return -1;
}

/// @cond DOXYGEN_SHOULD_SKIP_THIS
uart_config_t default_cfg = {.baud_rate = UART_DEFAULT_BAUD_RATE,
                             .clk_freq_hz = UART_CLK_FREQ_HZ,
                             .data_bits = UART_DEFAULT_DATA_BITS,
                             .parity = UART_DEFAULT_PARITY,
                             .stop_bits = UART_DEFAULT_STOP_BITS};
/// @endcond

/// @cond DOXYGEN_SHOULD_SKIP_THIS
__attribute__((weak)) chi_device_api_t uart_api = {
    .open = uart_open, .close = uart_close, .read = uart_read, .write = uart_write};
/// @endcond
