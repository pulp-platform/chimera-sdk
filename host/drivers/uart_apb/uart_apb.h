// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/**
 * \defgroup drivers_uart_apb UART APB Driver
 * @ingroup drivers_uart
 * @ingroup drivers
 * @brief APB UART register definitions for Chimera-SDK.
 * @{
 *
 * This file defines register offsets and fields for an APB-based UART peripheral.
 * It is used in conjunction with the UART driver to configure and interact with
 * the hardware.
 */

#ifndef UART_APB_H
#define UART_APB_H

#include "uart.h"

/**
 * @name UART Register Offsets
 * @brief Defines the register offsets for the APB UART peripheral.
 * @{
 */
#define UART_RBR_REG_OFFSET 0            /**< Receiver Buffer Register (RBR) Offset. */
#define UART_THR_REG_OFFSET 0            /**< Transmitter Holding Register (THR) Offset. */
#define UART_INTR_ENABLE_REG_OFFSET 4    /**< Interrupt Enable Register (IER) Offset. */
#define UART_INTR_IDENT_REG_OFFSET 8     /**< Interrupt Identification Register (IIR) Offset. */
#define UART_FIFO_CONTROL_REG_OFFSET 8   /**< FIFO Control Register (FCR) Offset. */
#define UART_LINE_CONTROL_REG_OFFSET 12  /**< Line Control Register (LCR) Offset. */
#define UART_MODEM_CONTROL_REG_OFFSET 16 /**< Modem Control Register (MCR) Offset. */
#define UART_LINE_STATUS_REG_OFFSET 20   /**< Line Status Register (LSR) Offset. */
#define UART_MODEM_STATUS_REG_OFFSET 24  /**< Modem Status Register (MSR) Offset. */
#define UART_DLAB_LSB_REG_OFFSET 0       /**< Divisor Latch LSB Register (DLAB_LSB) Offset. */
#define UART_DLAB_MSB_REG_OFFSET 4       /**< Divisor Latch MSB Register (DLAB_MSB) Offset. */
/** @} */                                // End of UART Register Offsets

/**
 * @name UART Line Status Register (LSR) Bit Definitions
 * @brief Defines the bit positions for the Line Status Register (LSR).
 * @{
 */
#define UART_LINE_STATUS_DATA_READY_BIT \
    0 /**< Data Ready (DR) bit. Set when data is available in RBR. */
#define UART_LINE_STATUS_THR_EMPTY_BIT 5 /**< Transmitter Holding Register (THR) empty bit. */
#define UART_LINE_STATUS_TMIT_EMPTY_BIT \
    6     /**< Transmitter empty bit. Set when both THR and shift register are empty. */
/** @} */ // End of UART LSR Bit Definitions

#endif // UART_APB_H

/** @} */ // End of drivers_uart_apb group
