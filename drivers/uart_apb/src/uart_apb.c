// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

/**
 * \addtogroup drivers
 * @{
 * \defgroup drivers_uart_apb UART APB Driver
 * @ingroup hal_interface
 * @{
 * @brief APB UART driver implementation for Chimera-SDK.
 *
 * This file provides the implementation of UART initialization, read, and write
 * functions for an APB-based UART peripheral. It includes blocking read and write
 * operations along with basic configuration.
 *
 */

#include "uart_apb.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Checks if data is available to read from the UART receiver.
 *
 * @param base Base address of the UART peripheral.
 * @return 1 if data is ready, 0 otherwise.
 */
static inline int rx_ready(uint32_t base) {
    uint8_t status = reg8_read(base, UART_LINE_STATUS_REG_OFFSET);
    return (status & (1 << UART_LINE_STATUS_DATA_READY_BIT)) != 0;
}

/**
 * @brief Checks if the transmitter is ready to accept new data.
 *
 * @param base Base address of the UART peripheral.
 * @return 1 if the transmitter is ready, 0 otherwise.
 */
static inline int tx_ready(uint32_t base) {
    uint8_t status = reg8_read(base, UART_LINE_STATUS_REG_OFFSET);
    return (status & (1 << UART_LINE_STATUS_THR_EMPTY_BIT)) != 0;
}

/**
 * @brief Opens and initializes the UART interface.
 *
 * @param iface UART interface instance.
 * @return 0 on success, -1 on failure.
 */
int uart_apb_open(chi_interface_t *iface) {
    if (!iface || !iface->base) {
        return -1;
    }

    // Use default config if none provided
    if (!iface->cfg) {
        iface->cfg = &default_cfg;
    }

    uart_config_t *cfg = (uart_config_t *)iface->cfg;
    uint32_t base = (uint32_t)iface->base;
    uint32_t divisor = cfg->clk_freq_hz / (cfg->baud_rate << 4);
    uint8_t dlo = (uint8_t)(divisor);
    uint8_t dhi = (uint8_t)(divisor >> 8);

    // 1. Disable all interrupts
    reg8_write(base, UART_INTR_ENABLE_REG_OFFSET, 0x00);

    // 2. Enable DLAB, set baud rate
    reg8_write(base, UART_LINE_CONTROL_REG_OFFSET, 0x80);
    reg8_write(base, UART_DLAB_LSB_REG_OFFSET, dlo);
    reg8_write(base, UART_DLAB_MSB_REG_OFFSET, dhi);

    // 3. Configure line control: 8 data bits, no parity, 1 stop bit
    reg8_write(base, UART_LINE_CONTROL_REG_OFFSET, 0x03);

    // 4. Enable & clear FIFO, 14-byte threshold
    reg8_write(base, UART_FIFO_CONTROL_REG_OFFSET, 0xC7);

    // 5. Enable auto-flow control
    reg8_write(base, UART_MODEM_CONTROL_REG_OFFSET, 0x20);

    return 0;
}

/**
 * @brief Closes the UART interface.
 *
 * @param iface UART interface instance.
 * @return 0 on success, -1 on failure.
 */
int uart_apb_close(chi_interface_t *iface) {
    if (!iface || !iface->base) {
        return -1;
    }

    uint32_t base = (uint32_t)iface->base;

    // Reset control registers
    reg8_write(base, UART_INTR_ENABLE_REG_OFFSET, 0x00);
    reg8_write(base, UART_LINE_CONTROL_REG_OFFSET, 0x00);
    reg8_write(base, UART_FIFO_CONTROL_REG_OFFSET, 0x00);
    reg8_write(base, UART_MODEM_CONTROL_REG_OFFSET, 0x00);

    return 0;
}

/**
 * @brief Reads data from the UART receiver (blocking mode).
 *
 * @param iface UART interface instance.
 * @param buffer Buffer to store received data.
 * @param size Number of bytes to read.
 * @param cb Optional callback function (set to NULL if not needed).
 * @return Number of bytes read on success, -1 on failure.
 */
ssize_t uart_apb_read(chi_interface_t *iface, void *buffer, uint32_t size,
                      chi_interface_callback_t cb) {
    if (!iface || !iface->base || !buffer || size == 0) {
        return -1;
    }

    uint8_t *dst = (uint8_t *)buffer;
    uint32_t base = (uint32_t)iface->base;

    for (uint32_t i = 0; i < size; i++) {
        while (!rx_ready(base)) {
        }
        dst[i] = reg8_read(base, UART_RBR_REG_OFFSET);
    }

    if (cb) {
        (void)cb(iface);
    }

    return (ssize_t)size;
}

/**
 * @brief Writes data to the UART transmitter (blocking mode).
 *
 * @param iface UART interface instance.
 * @param buffer Data to send.
 * @param size Number of bytes to write.
 * @param cb Optional callback function (set to NULL if not needed).
 * @return Number of bytes written on success, -1 on failure.
 */
ssize_t uart_apb_write(chi_interface_t *iface, const void *buffer, uint32_t size,
                       chi_interface_callback_t cb) {
    if (!iface || !iface->base || !buffer || size == 0) {
        return -1;
    }

    const uint8_t *src = (const uint8_t *)buffer;
    uint32_t base = (uint32_t)iface->base;

    for (uint32_t i = 0; i < size; i++) {
        while (!tx_ready(base)) {
        }
        reg8_write(base, UART_THR_REG_OFFSET, src[i]);
    }

    if (cb) {
        (void)cb(iface);
    }

    return (ssize_t)size;
}

// VIVIANEP: Need to skip doxygen generation for these functions
// to avoid duplicated defintion errors in the generated documentation

/// @cond DOXYGEN_SHOULD_SKIP_THIS
extern int uart_open(chi_interface_t *iface)
    __attribute__((alias("uart_apb_open"), used, visibility("default")));
extern int uart_close(chi_interface_t *iface)
    __attribute__((alias("uart_apb_close"), used, visibility("default")));
extern ssize_t uart_read(chi_interface_t *iface, void *buffer, uint32_t size,
                         chi_interface_callback_t cb)
    __attribute__((alias("uart_apb_read"), used, visibility("default")));
extern ssize_t uart_write(chi_interface_t *iface, const void *buffer, uint32_t size,
                          chi_interface_callback_t cb)
    __attribute__((alias("uart_apb_write"), used, visibility("default")));

chi_interface_api_t uart_api = {
    .open = uart_apb_open, .close = uart_apb_close, .read = uart_apb_read, .write = uart_apb_write};
/// @endcond

/** @} */ // End of drivers_uart_apb group
/** @} */ // End of drivers group
