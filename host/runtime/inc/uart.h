// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#pragma once

#ifdef CHIMERA_DRIVER_UART

// Include Standard Libraries
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers
#include "interface_api.h"

/**
 * \defgroup drivers_uart UART Driver
 * @ingroup runtime
 * @ingroup drivers
 * @brief Generic UART driver implementation for Chimera-SDK.
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
#define UART_DEFAULT_BAUD_RATE 9600 /**< Default baud rate (bps). */
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
extern const uart_config_t default_uart_cfg;

/**
 * @brief UART HAL API structure.
 *
 * This structure defines function pointers for UART operations, allowing for
 * modular driver implementations.
 *
 * @code
 * chi_interface_api_t uart_api = {
 *     .open  = uart_open,
 *     .close = uart_close,
 *     .read  = uart_read,
 *     .write = uart_write,
 *     .flush = uart_flush
 * };
 * @endcode
 */
extern const chi_interface_api_t default_uart_api;

extern chi_interface_t default_uart_inst;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the UART interface.
 * This function sets up the UART hardware with the specified configuration.
 */
void _uart_init(void);

/**
 * @brief De-initializes the UART interface.
 * This function releases any resources held by the UART driver.
 */
void _uart_deinit(void);

/**
 * @brief Puts a character to the UART.
 * @param c Character to send.
 * @param file Pointer to the FILE structure (unused).
 * @return The character written as an unsigned char cast to an int or EOF on error.
 */
int uart_putc(char c, FILE *file);

/**
 * @brief Gets a character from the UART.
 * @param file Pointer to the FILE structure (unused).
 * @return The character read as an unsigned char cast to an int or EOF on error.
 */
int uart_getc(FILE *file);

/**
 * @brief Flushes the UART output buffer.
 * @param file Pointer to the FILE structure (unused).
 * @return 0 on success, negative value on failure.
 */
int uart_flush(FILE *file);

#ifdef __cplusplus
}
#endif

/** @} */ // end defgroup drivers_uart

#endif // CHIMERA_DRIVER_UART
