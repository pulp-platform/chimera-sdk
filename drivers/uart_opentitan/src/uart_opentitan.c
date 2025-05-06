// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

/**
 * \addtogroup drivers
 * @{ \defgroup drivers_uart_opentitan UART OpenTitan Driver
 * @ingroup hal_interface
 * @{
 * @brief OpenTitan UART driver implementation for Chimera-SDK.
 *
 * This file defines the OpenTitan UART driver using the HAL interface.
 * It adapts the DIF-based implementation to the generic chi_interface API.
 */

#include "uart_opentitan.h"
#include "interface_api.h"
#include <stdlib.h>
#include <string.h>
#include "sw/device/lib/dif/dif_uart.h"

/**
 * @brief Opens and initializes the OpenTitan UART interface.
 *
 * @param iface UART interface instance.
 * @return 0 on success, -1 on error.
 */
int uart_opentitan_open(chi_interface_t *iface) {
    if (!iface || !iface->cfg) {
        return -1;
    }

    uart_config_t *config = (uart_config_t *)iface->cfg;
    opentitan_uart_context_t *context = (opentitan_uart_context_t *)config;

    dif_uart_config_t dif_cfg = {
        .baudrate = config->baud_rate,
        .clk_freq_hz = config->clk_freq_hz,
        .parity_enable =
            (config->parity != UART_PARITY_NONE) ? kDifToggleEnabled : kDifToggleDisabled,
        .parity = (config->parity == UART_PARITY_EVEN) ? kDifUartParityEven : kDifUartParityOdd,
    };

    dif_result_t result = dif_uart_configure(&context->uart, dif_cfg);
    if (result != kDifOk) {
        return -1;
    }
    return 0;
}

/**
 * @brief Closes the OpenTitan UART interface.
 *
 * @param iface UART interface instance.
 * @return 0
 */
int uart_opentitan_close(chi_interface_t *iface) {
    (void)iface;
    return 0;
}

/**
 * @brief Reads data from the OpenTitan UART.
 *
 * This is a blocking operation that only returns once the requested number of bytes
 * has been read or a read failure occurs. The optional callback, if provided, is
 * invoked immediately after the blocking read completes, serving as a notification
 * hook. The callback is **not** called asynchronously.
 *
 * @param iface UART interface instance.
 * @param buffer Buffer to store received data.
 * @param size Number of bytes to read.
 * @param cb Optional callback function (set to NULL if not needed).
 * @return Number of bytes read on success, -1 on error.
 */
ssize_t uart_opentitan_read(chi_interface_t *iface, void *buffer, uint32_t size,
                            chi_interface_callback_t cb) {
    if (!iface || !iface->cfg || !buffer || size == 0) {
        return -1;
    }
    opentitan_uart_context_t *context = (opentitan_uart_context_t *)iface->cfg;
    size_t read;
    dif_result_t result = dif_uart_bytes_receive(&context->uart, size, buffer, &read);
    if (result != kDifOk) {
        return -1;
    }
    if (cb) cb(iface);
    return (ssize_t)read;
}

/**
 * @brief Writes data to the OpenTitan UART transmitter.
 *
 * This is a blocking operation that only returns once the data has been written.
 * The optional callback, if provided, is invoked immediately after the blocking
 * write completes, serving as a notification hook. The callback is **not**
 * called asynchronously.
 *
 * @param iface UART interface instance.
 * @param buffer Data to send.
 * @param size Number of bytes to write.
 * @param cb Optional callback function (set to NULL if not needed).
 * @return Number of bytes written on success, -1 on error.
 */
ssize_t uart_opentitan_write(chi_interface_t *iface, const void *buffer, uint32_t size,
                             chi_interface_callback_t cb) {
    if (!iface || !iface->cfg || !buffer || size == 0) {
        return -1;
    }
    opentitan_uart_context_t *context = (opentitan_uart_context_t *)iface->cfg;
    size_t written;
    dif_result_t result = dif_uart_bytes_send(&context->uart, size, buffer, &written);
    if (result != kDifOk) {
        return -1;
    }
    if (cb) cb(iface);
    return (ssize_t)written;
}

// VIVIANEP: Need to skip doxygen generation for these functions
// to avoid duplicated defintion errors in the generated documentation

/// @cond DOXYGEN_SHOULD_SKIP_THIS
extern int uart_open(chi_interface_t *iface)
    __attribute__((alias("uart_opentitan_open"), used, visibility("default")));
extern int uart_close(chi_interface_t *iface)
    __attribute__((alias("uart_opentitan_close"), used, visibility("default")));
extern ssize_t uart_read(chi_interface_t *iface, void *buffer, uint32_t size,
                         chi_interface_callback_t cb)
    __attribute__((alias("uart_opentitan_read"), used, visibility("default")));
extern ssize_t uart_write(chi_interface_t *iface, const void *buffer, uint32_t size,
                          chi_interface_callback_t cb)
    __attribute__((alias("uart_opentitan_write"), used, visibility("default")));

chi_interface_api_t uart_api = {.open = uart_opentitan_open,
                                .close = uart_opentitan_close,
                                .read = uart_opentitan_read,
                                .write = uart_opentitan_write};
/// @endcond

/** @} */ // End of drivers_uart_opentitan group
/** @} */ // End of drivers group
