// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers
#include "uart.h"

#ifdef HARDWARE_BACKEND_ASIC
#ifdef CHIMERA_DRIVER_UART
int uart_putc(char c, FILE *file) {
    (void)file;
    default_uart_inst.api->write(&default_uart_inst, &c, 1, NULL);
    default_uart_inst.api->flush(&default_uart_inst);
    return c;
}
#else
#warning "UART driver not enabled for ASIC backend, using stub implementation."
int uart_putc(char c, FILE *file) {
    (void)c;
    (void)file;
    return -1;
}
#endif // CHIMERA_DRIVER_UART
#elif HARDWARE_BACKEND_RTL
int uart_putc(char c, FILE *file) {
    (void)c;
    *(volatile uint32_t *)(long)(0x300010F0) = c;
    return c;
}
#else  // HARDWARE_BACKEND_GVSOC
int uart_putc(char c, FILE *file) {
    (void)file;
    *(volatile uint32_t *)(long)(0x03004000) = c;
    return c;
}
#endif // HARDWARE_BACKEND_RTL
