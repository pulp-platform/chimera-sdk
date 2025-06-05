// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#include "uart.h"

/*---------------------------------------------------------------------------*/
/* UART HAL weak stubs (overrideable by target-specific drivers)             */
/*---------------------------------------------------------------------------*/

__attribute__((weak)) int uart_open(chi_interface_t *iface) {
    if (!iface || !iface->base) {
        return -1;
    }
    return 0;
}

__attribute__((weak)) int uart_close(chi_interface_t *iface) {
    if (!iface || !iface->base) {
        return -1;
    }
    return 0;
}

__attribute__((weak)) ssize_t uart_read(chi_interface_t *iface, void *buffer, uint32_t size,
                                        chi_interface_callback_t cb) {
    if (!iface || !iface->base || !buffer || size == 0) {
        return -1;
    }
    return -1;
}

__attribute__((weak)) ssize_t uart_write(chi_interface_t *iface, const void *buffer, uint32_t size,
                                         chi_interface_callback_t cb) {
    if (!iface || !iface->base || !buffer || size == 0) {
        return -1;
    }
    return -1;
}

/*---------------------------------------------------------------------------*/
/* Skip documenting these symbols in Doxygen to avoid duplicate entries      */
/*---------------------------------------------------------------------------*/

/// @cond DOXYGEN_SHOULD_SKIP_THIS
const uart_config_t default_cfg = {.baud_rate = UART_DEFAULT_BAUD_RATE,
                                   .clk_freq_hz = UART_CLK_FREQ_HZ,
                                   .data_bits = UART_DEFAULT_DATA_BITS,
                                   .parity = UART_DEFAULT_PARITY,
                                   .stop_bits = UART_DEFAULT_STOP_BITS};

__attribute__((weak)) chi_interface_api_t uart_api = {
    .open = uart_open, .close = uart_close, .read = uart_read, .write = uart_write};
/// @endcond
