// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h> // for ssize_t
#include "interface_api.h"

/**
 * @defgroup hal_uart UART HAL Interface
 * @ingroup hal_interface
 * @brief UART HAL interface for Chimera-SDK.
 *
 * This module defines the UART configuration structure and HAL API functions
 * for interfacing with different UART drivers in Chimera-SDK.
 * @{
 */

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
 * @brief Opens and initializes the UART interface.
 *
 * @param iface Pointer to the UART interface instance.
 * @return 0 on success, negative value on failure.
 */
extern int uart_open(chi_interface_t *iface);

/**
 * @brief Closes the UART interface.
 *
 * @param iface Pointer to the UART interface instance.
 * @return 0 on success, negative value on failure.
 */
extern int uart_close(chi_interface_t *iface);

/**
 * @brief Reads data from the UART receiver.
 *
 * @param iface  Pointer to the UART interface instance.
 * @param buffer Pointer to the buffer where received data will be stored.
 * @param size   Number of bytes to read.
 * @param cb     Optional callback function to signal completion.
 * @return Number of bytes read on success, negative value on failure.
 */
extern ssize_t uart_read(chi_interface_t *iface, void *buffer, uint32_t size,
                         chi_interface_callback_t cb);

/**
 * @brief Writes data to the UART transmitter.
 *
 * @param iface  Pointer to the UART interface instance.
 * @param buffer Pointer to the data to send.
 * @param size   Number of bytes to write.
 * @param cb     Optional callback function to signal completion.
 * @return Number of bytes written on success, negative value on failure.
 */
extern ssize_t uart_write(chi_interface_t *iface, const void *buffer, uint32_t size,
                          chi_interface_callback_t cb);

/**
 * @brief Default UART configuration settings.
 *
 * This global configuration structure defines default parameters for UART communication,
 * which can be overridden by the user.
 *
 * **Default Configuration:**
 * @code
 * uart_config_t default_cfg = {
 *     .baud_rate   = UART_DEFAULT_BAUD_RATE,
 *     .clk_freq_hz = UART_CLK_FREQ_HZ,
 *     .data_bits   = UART_DEFAULT_DATA_BITS,
 *     .parity      = UART_DEFAULT_PARITY,
 *     .stop_bits   = UART_DEFAULT_STOP_BITS
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
 * __attribute__((weak)) chi_interface_api_t uart_api = {
 *     .open  = uart_open,
 *     .close = uart_close,
 *     .read  = uart_read,
 *     .write = uart_write
 * };
 * @endcode
 */
extern chi_interface_api_t uart_api;

/** @} */ // end defgroup hal_uart

#endif // UART_H
