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
 * @file uart_opentitan.h
 * @brief OpenTitan-specific UART driver interface for Chimera-SDK.
 *
 * This file defines the UART context structure for OpenTitan-based UART
 * implementations. It provides an abstraction layer using OpenTitan's
 * DIF (Device Interface Functions) for UART communication.
 *
 * @warning This driver has not been tested yet. Chimera currently supports
 *          the `uart_apb` driver, and this implementation is still a work in
 *          progress (WIP). Use with caution.
 *
 */

#ifndef UART_OPENTITAN_H
#define UART_OPENTITAN_H

#include <stdint.h>
#include "uart.h"
#include "sw/device/lib/dif/dif_uart.h"

/**
 * \ingroup drivers_uart_opentitan
 * @brief OpenTitan-specific UART context.
 *
 * This structure holds the device interface function (DIF) handle
 * for OpenTitan-based UART communication.
 */
typedef struct {
    dif_uart_t uart; /**< OpenTitan DIF UART handle. */
} opentitan_uart_context_t;

#endif // UART_OPENTITAN_H

/** @} */ // End of drivers_uart_opentitan group
/** @} */ // End of drivers group
