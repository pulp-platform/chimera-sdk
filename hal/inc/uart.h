// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

/**
 * @defgroup hal Hardware Abstraction Layer (HAL)
 * @brief The HAL module provides a unified API for hardware peripherals.
 * @{
 */

/**
 * @defgroup hal_uart UART HAL Interface
 * @ingroup hal
 * @brief UART HAL interface for Chimera-SDK.
 *
 * This module defines the UART configuration structure and HAL API functions
 * for interfacing with different UART drivers in Chimera-SDK.
 *
 * @{
 *
 * @file uart.h
 * @author Viviane Potocnik <vivianep@iis.ee.ethz.ch>
 * @date 2025-02-05
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <sys/types.h> // Needed for ssize_t
#include "device_api.h"

/** @name UART Parity Modes
 *  @brief Defines available parity options for UART.
 *  @{
 */
#define UART_PARITY_NONE 0 /**< No parity. */
#define UART_PARITY_EVEN 1 /**< Even parity. */
#define UART_PARITY_ODD 2  /**< Odd parity. */
/** @} */

/** @name Default UART Configurations
 *  @brief Defines default UART settings, which can be overridden at compile-time.
 *  @{
 */
#ifndef UART_DEFAULT_BAUD_RATE
#define UART_DEFAULT_BAUD_RATE 115200 /**< Default baud rate (bps). */
#endif

#ifndef UART_DEFAULT_DATA_BITS
#define UART_DEFAULT_DATA_BITS 8 /**< Default number of data bits. */
#endif

#ifndef UART_DEFAULT_PARITY
#define UART_DEFAULT_PARITY UART_PARITY_NONE /**< Default parity mode. */
#endif

#ifndef UART_DEFAULT_STOP_BITS
#define UART_DEFAULT_STOP_BITS 1 /**< Default number of stop bits. */
#endif

#ifndef UART_CLK_FREQ_HZ
#define UART_CLK_FREQ_HZ 100000000 /**< Default clock frequency in Hz. */
#endif

#ifndef UART_BASE_ADDR
#define UART_BASE_ADDR 0x40000000 /**< Default base address for UART. */
#endif
/** @} */

/**
 * @brief UART configuration structure.
 *
 * This structure holds the configuration settings for UART initialization.
 */
typedef struct {
    uint32_t baud_rate;   /**< Baud rate in bits per second. */
    uint32_t clk_freq_hz; /**< Clock frequency in Hz. */
    uint8_t data_bits;    /**< Number of data bits (typically 8). */
    uint8_t parity;       /**< Parity setting (None, Even, Odd). */
    uint8_t stop_bits;    /**< Number of stop bits (1 or 2). */
} uart_config_t;

/**
 * @brief Opens and initializes the UART device.
 *
 * @param device Pointer to the UART device structure.
 * @return 0 on success, negative value on failure.
 */
extern int uart_open(chi_device_t *device);

/**
 * @brief Closes the UART device.
 *
 * @param device Pointer to the UART device structure.
 * @return 0 on success, negative value on failure.
 */
extern int uart_close(chi_device_t *device);

/**
 * @brief Reads data from the UART receiver.
 *
 * @param device Pointer to the UART device structure.
 * @param buffer Pointer to the buffer where received data will be stored.
 * @param size Number of bytes to read.
 * @param cb Optional callback function to signal completion.
 * @return Number of bytes read on success, negative value on failure.
 */
extern ssize_t uart_read(chi_device_t *device, void *buffer, uint32_t size,
                         chi_device_callback_t cb);

/**
 * @brief Writes data to the UART transmitter.
 *
 * @param device Pointer to the UART device structure.
 * @param buffer Pointer to the data to send.
 * @param size Number of bytes to write.
 * @param cb Optional callback function to signal completion.
 * @return Number of bytes written on success, negative value on failure.
 */
extern ssize_t uart_write(chi_device_t *device, const void *buffer, uint32_t size,
                          chi_device_callback_t cb);

/**
 * @brief Default UART configuration settings.
 *
 * This global configuration structure defines default parameters for UART communication,
 * which can be overridden by the user.
 *
 * **Default Configuration:**
 * @code
 * uart_config_t default_cfg = {
 *     .baud_rate   = UART_DEFAULT_BAUD_RATE,  // Default baud rate
 *     .clk_freq_hz = UART_CLK_FREQ_HZ,        // Default clock frequency
 *     .data_bits   = UART_DEFAULT_DATA_BITS,  // Default number of data bits
 *     .parity      = UART_DEFAULT_PARITY,     // Default parity setting
 *     .stop_bits   = UART_DEFAULT_STOP_BITS   // Default number of stop bits
 * };
 * @endcode
 */
extern uart_config_t default_cfg;

/**
 * @brief UART HAL API structure.
 *
 * This structure defines function pointers for UART operations, allowing for
 * modular driver implementations.
 *
 * The default implementation uses weak symbols that can be overridden by specific drivers.
 *
 * @code
 * __attribute__((weak)) chi_device_api_t uart_api = {
 *     .open  = uart_open,
 *     .close = uart_close,
 *     .read  = uart_read,
 *     .write = uart_write
 * };
 * @endcode
 */
extern chi_device_api_t uart_api;

#endif // UART_H

/** @} */ // End of hal_uart group
/** @} */ // End of hal group
