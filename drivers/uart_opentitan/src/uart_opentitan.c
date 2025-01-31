// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#include "uart_opentitan.h"
#include <stdlib.h>
#include <string.h>

// OpenTitan UART open function
int uart_open(struct chi_device *device) {
    if (!device || !device->device_addr || !device->cfg) {
        return -1; // Invalid argument
    }

    // Retrieve the device-specific configuration
    uart_config_t *config = (uart_config_t *)device->cfg;

    // Allocate memory for OpenTitan-specific UART context
    opentitan_uart_context_t *context =
        (opentitan_uart_context_t *)malloc(sizeof(opentitan_uart_context_t));
    if (!context) {
        return -2; // Memory allocation failed
    }

    // Configure the UART using the retrieved configuration
    dif_uart_config_t dif_config = {
        .baudrate = config->baud_rate,
        .clk_freq_hz = config->clk_freq_hz,
        .parity_enable =
            (config->parity != UART_PARITY_NONE) ? kDifToggleEnabled : kDifToggleDisabled,
        .parity = (config->parity == UART_PARITY_EVEN) ? kDifUartParityEven : kDifUartParityOdd,
    };

    // Initialize the UART using the OpenTitan DIF
    dif_result_t result = dif_uart_configure(&context->uart, dif_config);
    if (result != kDifOk) {
        free(context);
        return -3; // Configuration failed
    }

    // Save the driver-specific context in the device's cfg field
    device->cfg = context;

    return 0; // Success
}

// OpenTitan UART close function
int uart_close(struct chi_device *device) {
    if (!device || !device->cfg) {
        return -1; // Invalid argument
    }

    // Free the allocated context
    free(device->cfg);
    device->cfg = NULL;
    return 0; // Success
}

// OpenTitan UART asynchronous read
ssize_t uart_read(struct chi_device *device, void *buffer, uint32_t size, chi_device_callback cb) {
    if (!device || !device->cfg || !buffer || size == 0) {
        return -1; // Invalid argument
    }

    opentitan_uart_context_t *context = (opentitan_uart_context_t *)device->cfg;

    size_t bytes_read = 0;
    dif_result_t result = dif_uart_bytes_receive(&context->uart, size, buffer, &bytes_read);
    if (result != kDifOk) {
        return -2; // Read failed
    }

    if (cb) {
        cb(device); // Invoke the callback
    }

    return bytes_read;
}

// OpenTitan UART asynchronous write
ssize_t uart_write(struct chi_device *device, const void *buffer, uint32_t size,
                   chi_device_callback cb) {
    if (!device || !device->cfg || !buffer || size == 0) {
        return -1; // Invalid argument
    }

    opentitan_uart_context_t *context = (opentitan_uart_context_t *)device->cfg;

    size_t bytes_written = 0;
    dif_result_t result = dif_uart_bytes_send(&context->uart, size, buffer, &bytes_written);
    if (result != kDifOk) {
        return -2; // Write failed
    }

    if (cb) {
        cb(device); // Invoke the callback
    }

    return bytes_written;
}
