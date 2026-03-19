// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/*
 * UART instance and backend-specific initialisation.
 *
 * default_uart_cfg and default_uart_inst are the module-level singletons used
 * by the rest of the runtime (putc.c, stdio.c, fll.c).
 *
 * _uart_init() is conditional on HARDWARE_BACKEND:
 *  - GVSOC / RTL:  no-op (the simulator handles UART internally).
 *  - ASIC:         measures the actual core clock via CLINT before opening the
 *                  UART so that the baud divisor is computed from the real
 *                  frequency rather than the compile-time default.
 */

#ifdef CHIMERA_DRIVER_UART

// Include Standard Libraries

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers

// Include Runtime Headers
#include "util.h"
#include "uart.h"
#include "clint.h"

const uart_config_t default_uart_cfg = {.baud_rate = UART_DEFAULT_BAUD_RATE,
                                        .clk_freq_hz = UART_CLK_FREQ_HZ,
                                        .data_bits = UART_DEFAULT_DATA_BITS,
                                        .parity = UART_DEFAULT_PARITY,
                                        .stop_bits = UART_DEFAULT_STOP_BITS};

chi_interface_t default_uart_inst = {
    .api = &default_uart_api, .base = (uintptr_t)&__base_uart, .cfg = (void *)&default_uart_cfg};

#ifdef HARDWARE_BACKEND_GVSOC
void _uart_init(void) {
}
void _uart_deinit(void) {
}
#elif HARDWARE_BACKEND_RTL
void _uart_init(void) {
}
void _uart_deinit(void) {
}
#else  // HARDWARE_BACKEND_GVSOC
void _uart_init(void) {
    static uart_config_t uart_cfg;
    uart_cfg = default_uart_cfg;

    uint32_t rtc_freq = *reg32(&__base_regs, CHESHIRE_RTC_FREQ_REG_OFFSET);
    uint32_t reset_freq = clint_get_core_freq(rtc_freq, 512);
    uart_cfg.clk_freq_hz = reset_freq;

    default_uart_inst.cfg = &uart_cfg;

    default_uart_inst.api->open(&default_uart_inst);
}

void _uart_deinit(void) {
    default_uart_inst.api->close(&default_uart_inst);
}
#endif // HARDWARE_BACKEND_GVSOC

#endif // CHIMERA_DRIVER_UART