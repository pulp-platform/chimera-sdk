// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

/**
 * \addtogroup drivers
 * @{
 * \defgroup drivers_uart_opentitan UART OpenTitan Driver
 * @{
 *
 * @file uart_opentitan.c
 * @brief OpenTitan UART driver implementation for Chimera-SDK.
 *
 * @warning This driver has not been fully validated yet. Chimera currently supports
 *          the `uart_apb` driver, and this implementation is still work in progress (WIP).
 *          Use with caution.
 *
 * This file provides the implementation of UART initialization, read, and write
 * functions using OpenTitan's Device Interface Functions (DIF).
 *
 * @author Viviane Potocnik vivianep@iis.ee.ethz.ch
 * @date 2025-02-05
 */

#include "uart_opentitan.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Opens and initializes the OpenTitan UART device.
 *
 * @param device Pointer to the UART device.
 * @return 0 on success, -1 on invalid argument, -2 on memory allocation failure, -3 on
 * configuration failure.
 */
int uart_opentitan_open(chi_device_t *device) {
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

/**
 * @brief Closes the OpenTitan UART device.
 *
 * @param device Pointer to the UART device.
 * @return 0 on success, -1 on invalid argument.
 */
int uart_opentitan_close(chi_device_t *device) {
    if (!device || !device->cfg) {
        return -1; // Invalid argument
    }

    // Free the allocated context
    free(device->cfg);
    device->cfg = NULL;
    return 0; // Success
}

/**
 * @brief Reads data from the OpenTitan UART receiver.
 *
 * @param device Pointer to the UART device.
 * @param buffer Buffer to store received data.
 * @param size Number of bytes to read.
 * @param cb Optional callback function (set to NULL if not needed).
 * @return Number of bytes read on success, -1 on invalid argument, -2 on read failure.
 */
ssize_t uart_opentitan_read(chi_device_t *device, void *buffer, uint32_t size,
                            chi_device_callback_t cb) {
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

/**
 * @brief Writes data to the OpenTitan UART transmitter.
 *
 * @param device Pointer to the UART device.
 * @param buffer Data to send.
 * @param size Number of bytes to write.
 * @param cb Optional callback function (set to NULL if not needed).
 * @return Number of bytes written on success, -1 on invalid argument, -2 on write failure.
 */
ssize_t uart_opentitan_write(chi_device_t *device, const void *buffer, uint32_t size,
                             chi_device_callback_t cb) {
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

// VIVIANEP: Need to skip doxygen generation for these functions
// to avoid duplicated defintion errors in the generated documentation

/// @cond DOXYGEN_SHOULD_SKIP_THIS
extern int uart_open(chi_device_t *device)
    __attribute__((alias("uart_opentitan_open"), used, visibility("default")));
extern int uart_close(chi_device_t *device)
    __attribute__((alias("uart_opentitan_close"), used, visibility("default")));
extern ssize_t uart_read(chi_device_t *device, void *buffer, uint32_t size,
                         chi_device_callback_t cb)
    __attribute__((alias("uart_opentitan_read"), used, visibility("default")));
extern ssize_t uart_write(chi_device_t *device, const void *buffer, uint32_t size,
                          chi_device_callback_t cb)
    __attribute__((alias("uart_opentitan_write"), used, visibility("default")));
// @endcond

/// @cond DOXYGEN_SHOULD_SKIP_THIS
chi_device_api_t uart_api = {.open = uart_opentitan_open,
                             .close = uart_opentitan_close,
                             .read = uart_opentitan_read,
                             .write = uart_opentitan_write};
// @endcond

/** @} */ // End of drivers_uart_opentitan group
/** @} */ // End of drivers group
