// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potcnik <vivianep@iis.ee.ethz.ch>

/**
 * \addtogroup hal_uart
 * @ingroup hal
 * @brief UART HAL interface for Chimera-SDK.
 *
 * This module defines the UART configuration structure and HAL API functions
 * for interfacing with different UART drivers in Chimera-SDK.
 *
 * @{
 *
 * @file uart.h
 * @author Viviane Potocnik
 * @email vivianep@iis.ee.ethz.ch
 * @date 2025-01-31
 * @license Apache-2.0
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdlib.h>
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
#define UART_DEFAULT_DATA_BITS 8 /**< Default data bits. */
#endif

#ifndef UART_DEFAULT_PARITY
#define UART_DEFAULT_PARITY UART_PARITY_NONE /**< Default parity mode. */
#endif

#ifndef UART_DEFAULT_STOP_BITS
#define UART_DEFAULT_STOP_BITS 1 /**< Default stop bits. */
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
 * @brief UART context structure.
 *
 * This structure holds pointers to the driver-specific context and API functions.
 */
typedef struct uart_context {
    void *driver_context; /**< Pointer to driver-specific context. */
    void *driver_api;     /**< Pointer to driver-specific API functions. */
} uart_context_t;

/**
 * @brief Opens and initializes the UART device.
 *
 * @param device Pointer to the UART device.
 * @return 0 on success, negative value on failure.
 */
extern int uart_open(struct chi_device *device);

/**
 * @brief Closes the UART device.
 *
 * @param device Pointer to the UART device.
 * @return 0 on success, negative value on failure.
 */
extern int uart_close(struct chi_device *device);

/**
 * @brief Reads data from the UART receiver (asynchronous).
 *
 * @param device Pointer to the UART device.
 * @param buffer Buffer to store received data.
 * @param size Number of bytes to read.
 * @param cb Optional callback function.
 * @return Number of bytes read on success, negative value on failure.
 */
extern ssize_t uart_read(struct chi_device *device, void *buffer, uint32_t size,
                         chi_device_callback cb);

/**
 * @brief Writes data to the UART transmitter (asynchronous).
 *
 * @param device Pointer to the UART device.
 * @param buffer Data to send.
 * @param size Number of bytes to write.
 * @param cb Optional callback function.
 * @return Number of bytes written on success, negative value on failure.
 */
extern ssize_t uart_write(struct chi_device *device, const void *buffer, uint32_t size,
                          chi_device_callback cb);

/** @brief UART API structure, defining function pointers for driver operations. */
extern struct chi_device_api uart_api;

/** @brief Default UART configuration settings. */
extern uart_config_t default_cfg;

#endif // UART_H

/** @} */ // End of hal_uart group
