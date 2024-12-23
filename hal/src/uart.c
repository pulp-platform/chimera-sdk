// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potcnik <vivianep@iis.ee.ethz.ch>

#include "uart.h"

// Define the UART API structure
struct chi_device_api uart_api = {
    .open = uart_open,
    .close = uart_close,
    .read_async = uart_read_async,
    .write_async = uart_write_async
};

// UART context structure
typedef struct uart_context {
    dif_uart_t uart; // UART DIF handle
    // Asynchronous read state
    uint8_t *rx_buffer;
    uint32_t rx_size;
    uint32_t rx_pos;
    chi_device_callback rx_callback;
    // Asynchronous write state
    const uint8_t *tx_buffer;
    uint32_t tx_size;
    uint32_t tx_pos;
    chi_device_callback tx_callback;
} uart_context_t;

// Default UART configuration
uart_config_t default_cfg = {
    .baud_rate = UART_DEFAULT_BAUD_RATE,
    .clk_freq_hz = UART_CLK_FREQ_HZ,
    .data_bits = UART_DEFAULT_DATA_BITS,
    .parity    = UART_DEFAULT_PARITY,
    .stop_bits = UART_DEFAULT_STOP_BITS
};

int uart_open(struct chi_device *device) {
    
    // Check if the device and device address are valid
    if (device == NULL || device->device_addr == NULL) {
        return -1; // Invalid argument
    }

    // Initialize the UART context
    uart_context_t *ctx = malloc(sizeof(uart_context_t));
    if (ctx == NULL) {
        return -2; // Out of memory error
    }
    memset(ctx, 0, sizeof(uart_context_t));

    // // If no cfg is provided, use the default configuration
    // uart_config_t *cfg = device->cfg;
    // if (cfg == NULL) {
    //     cfg = &default_cfg;
    // }

    // free(ctx); // Free the UART context

    return 0; // Success
}

int uart_close(struct chi_device *device) {
    
    if (device == NULL || device->cfg == NULL) {
        return -1; // Invalid argument
    }

    return 0; // Success
}

ssize_t uart_read_async(struct chi_device *device, void *buffer, uint32_t size, chi_device_callback cb) {
    
    if (device == NULL || device->cfg == NULL || buffer == NULL || size == 0) {
        return -1; // Invalid argument
    }

    return size; // Return the number of bytes to be read
}

ssize_t uart_write_async(struct chi_device *device, const void *buffer, uint32_t size, chi_device_callback cb) {
    
    if (device == NULL || device->cfg == NULL || buffer == NULL || size == 0) {
        return -1; // Invalid argument
    }

    return size; // Return the number of bytes to be written
}