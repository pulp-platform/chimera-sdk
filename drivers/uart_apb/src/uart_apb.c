// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

/**
 * \addtogroup drivers_uart_apb
 * @ingroup drivers
 * @{
 *
 * @file uart_apb.c
 * @brief APB UART driver implementation for Chimera-SDK.
 *
 * This file provides the implementation of UART initialization, read, and write
 * functions for an APB-based UART peripheral. It includes blocking read and write
 * operations along with basic configuration.
 *
 * @author Viviane Potocnik
 * @email vivianep@iis.ee.ethz.ch
 * @date 2025-01-31
 * @license Apache-2.0
 */

#include "uart_apb.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Checks if data is available to read from the UART receiver.
 * @internal
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
 * @internal
 *
 * @param base Base address of the UART peripheral.
 * @return 1 if the transmitter is ready, 0 otherwise.
 */
static inline int tx_ready(uint32_t base) {
    uint8_t status = reg8_read(base, UART_LINE_STATUS_REG_OFFSET);
    return (status & (1 << UART_LINE_STATUS_THR_EMPTY_BIT)) != 0;
}

/**
 * @brief Checks if the UART transmitter is idle (empty).
 * @internal
 *
 * @param base Base address of the UART peripheral.
 * @return 1 if the transmitter is idle, 0 otherwise.
 */
static inline int tx_idle(uint32_t base) {
    uint8_t status = reg8_read(base, UART_LINE_STATUS_REG_OFFSET);
    return (status & (1 << UART_LINE_STATUS_THR_EMPTY_BIT)) &&
           (status & (1 << UART_LINE_STATUS_TMIT_EMPTY_BIT));
}

/**
 * @brief Opens and initializes the UART device.
 *
 * This function configures the UART peripheral with the given settings or
 * uses default values if no configuration is provided.
 *
 * @param device Pointer to the UART device.
 * @return 0 on success, -1 if invalid arguments are provided.
 */
int uart_open(struct chi_device *device) {
    if (!device || !device->device_addr) {
        return -1;
    }

    // Use default config if none is provided
    if (!device->cfg) {
        device->cfg = &default_cfg;
    }

    uart_config_t *cfg = (uart_config_t *)device->cfg;
    uint32_t base = (uint32_t)device->device_addr;
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
 * @brief Closes the UART device.
 *
 * This function disables the UART peripheral by resetting control registers.
 *
 * @param device Pointer to the UART device.
 * @return 0 on success, -1 if invalid arguments are provided.
 */
int uart_close(struct chi_device *device) {
    if (!device || !device->device_addr) {
        return -1;
    }

    uint32_t base = (uint32_t)device->device_addr;

    // 1. Disable all interrupts
    reg8_write(base, UART_INTR_ENABLE_REG_OFFSET, 0x00);

    // 2. Disable DLAB
    reg8_write(base, UART_LINE_CONTROL_REG_OFFSET, 0x00);

    // 3. Reset FIFO control
    reg8_write(base, UART_FIFO_CONTROL_REG_OFFSET, 0x00);

    // 4. Reset modem control
    reg8_write(base, UART_MODEM_CONTROL_REG_OFFSET, 0x00);

    return 0;
}

/** @} */ // End of drivers_uart_apb group
