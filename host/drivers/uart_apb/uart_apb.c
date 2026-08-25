// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/*
 * APB UART driver implementation.
 *
 * uart_apb_open() calculates the baud-rate divisor from cfg->clk_freq_hz / baud_rate,
 * then programs the UART_CTRL register with data-bits, parity, stop-bits, and the
 * enable bit in a single write.
 *
 * uart_apb_write() spins on the TX FIFO full flag before writing each byte;
 * uart_apb_read() spins on the RX FIFO empty flag before reading.  Both are
 * blocking — no timeout or DMA path is provided.
 *
 * The chi_interface_api_t vtable (default_uart_api) is defined at the bottom and
 * referenced by default_uart_inst in uart.c.
 */

// Include Standard Libraries
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "uart_apb.h"

// Include Runtime Headers
#include "util.h"

// Import HAL Headers
#include "interface_api.h"

/* Returns 1 if the RX FIFO contains data ready to read, 0 otherwise. */
static inline int rx_ready(uint32_t base) {
    uint8_t status = reg8_read(base, UART_LINE_STATUS_REG_OFFSET);
    return (status & (1 << UART_LINE_STATUS_DATA_READY_BIT)) != 0;
}

/* Returns 1 if the TX holding register is empty and ready for a new byte, 0 otherwise. */
static inline int tx_ready(uint32_t base) {
    uint8_t status = reg8_read(base, UART_LINE_STATUS_REG_OFFSET);
    return (status & (1 << UART_LINE_STATUS_THR_EMPTY_BIT)) != 0;
}

/* Returns 1 if both the TX holding register and TX shift register are empty, 0 otherwise. */
static inline int tx_empty(uint32_t base) {
    uint8_t status = reg8_read(base, UART_LINE_STATUS_REG_OFFSET);
    return (status & (1 << UART_LINE_STATUS_TMIT_EMPTY_BIT)) != 0;
}

/* Blocks until the TX shift register is fully empty, ensuring all bytes have been transmitted. */
int uart_apb_flush(const chi_interface_t *iface) {
    if (!iface || !iface->base) {
        return -1;
    }

    uint32_t base = (uint32_t)iface->base;

    while (!tx_empty(base));

    return 0;
}

/* Configures baud rate, line control (8N1), FIFO, and flow control registers. */
int uart_apb_open(const chi_interface_t *iface) {
    if (!iface || !iface->base) {
        return -1;
    }

    uart_config_t *cfg = (uart_config_t *)iface->cfg;
    uint32_t base = (uint32_t)iface->base;
    uint32_t divisor = cfg->clk_freq_hz / (cfg->baud_rate << 4);
    uint8_t dlo = (uint8_t)(divisor);
    uint8_t dhi = (uint8_t)(divisor >> 8);

    // 1. Disable all interrupts
    reg8_write(base, UART_INTR_ENABLE_REG_OFFSET, 0x00);

    // 2. Enable DLAB, set baud rate
    reg8_write(base, UART_LINE_CONTROL_REG_OFFSET, 0x80);
    reg8_write(base, UART_DLAB_LSB_REG_OFFSET, dlo);
    reg8_write(base, UART_DLAB_MSB_REG_OFFSET, dhi);

    // 3. Configure line control: 8 data bits, no parity, 1 stop bit
    reg8_write(base, UART_LINE_CONTROL_REG_OFFSET, 0x03);

    // 4. Enable & clear FIFO, 14-byte threshold
    reg8_write(base, UART_FIFO_CONTROL_REG_OFFSET, 0xC7);

    // 5. Enable auto-flow control
    reg8_write(base, UART_MODEM_CONTROL_REG_OFFSET, 0x20);

    return 0;
}

/* Resets all UART control registers to disable the peripheral. */
int uart_apb_close(const chi_interface_t *iface) {
    if (!iface || !iface->base) {
        return -1;
    }

    uint32_t base = (uint32_t)iface->base;

    // Reset control registers
    reg8_write(base, UART_INTR_ENABLE_REG_OFFSET, 0x00);
    reg8_write(base, UART_LINE_CONTROL_REG_OFFSET, 0x00);
    reg8_write(base, UART_FIFO_CONTROL_REG_OFFSET, 0x00);
    reg8_write(base, UART_MODEM_CONTROL_REG_OFFSET, 0x00);

    return 0;
}

/* Blocking read of size bytes into buffer; polls RX ready before each byte. */
ssize_t uart_apb_read(const chi_interface_t *iface, void *buffer, uint32_t size,
                      chi_interface_callback_t cb) {
    if (!iface || !iface->base || !buffer || size == 0) {
        return -1;
    }

    uint8_t *dst = (uint8_t *)buffer;
    uint32_t base = (uint32_t)iface->base;

    for (uint32_t i = 0; i < size; i++) {
        while (!rx_ready(base)) {
        }
        dst[i] = reg8_read(base, UART_RBR_REG_OFFSET);
    }

    if (cb) {
        (void)cb(iface);
    }

    return (ssize_t)size;
}

/* Blocking write of size bytes from buffer; polls TX ready before each byte. */
ssize_t uart_apb_write(const chi_interface_t *iface, const void *buffer, uint32_t size,
                       chi_interface_callback_t cb) {
    if (!iface || !iface->base || !buffer || size == 0) {
        return -1;
    }

    const uint8_t *src = (const uint8_t *)buffer;
    uint32_t base = (uint32_t)iface->base;

    for (uint32_t i = 0; i < size; i++) {
        while (!tx_ready(base)) {
        }
        reg8_write(base, UART_THR_REG_OFFSET, src[i]);
    }

    // Make sure all data is transmitted
    while (!tx_ready(base));

    if (cb) {
        (void)cb(iface);
    }

    return (ssize_t)size;
}

const chi_interface_api_t default_uart_api = {.open = uart_apb_open,
                                              .close = uart_apb_close,
                                              .read = uart_apb_read,
                                              .write = uart_apb_write,
                                              .flush = uart_apb_flush};
