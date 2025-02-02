// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

/**
 * \addtogroup hal_uart
 * @ingroup hal
 * @{
 *
 * @file uart.c
 * @brief UART HAL implementation for Chimera-SDK.
 *
 * This file defines the default UART configuration and exposes the `uart_api`
 * structure, which provides function pointers for the UART HAL interface.
 *
 * @author Viviane Potocnik
 * @email vivianep@iis.ee.ethz.ch
 * @date 2025-01-31
 * @license Apache-2.0
 */

#include "uart.h"

/**
 * @brief UART HAL API structure.
 *
 * This structure provides function pointers for common UART operations.
 */
struct chi_device_api uart_api = {
    .open = uart_open,        /**< Pointer to the UART open function. */
    .close = uart_close,      /**< Pointer to the UART close function. */
    .read_async = uart_read,  /**< Pointer to the asynchronous UART read function. */
    .write_async = uart_write /**< Pointer to the asynchronous UART write function. */
};

/**
 * @brief Default UART configuration.
 *
 * Defines the default settings for UART communication, which can be overridden
 * by the user if necessary.
 */
uart_config_t default_cfg = {
    .baud_rate = UART_DEFAULT_BAUD_RATE, /**< Default baud rate in bits per second. */
    .clk_freq_hz = UART_CLK_FREQ_HZ,     /**< Default clock frequency in Hz. */
    .data_bits = UART_DEFAULT_DATA_BITS, /**< Default number of data bits (typically 8). */
    .parity = UART_DEFAULT_PARITY,       /**< Default parity mode (None, Even, Odd). */
    .stop_bits = UART_DEFAULT_STOP_BITS  /**< Default number of stop bits (1 or 2). */
};

/** @} */ // End of hal_uart group
