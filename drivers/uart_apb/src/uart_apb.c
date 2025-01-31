// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#include "uart_apb.h"
#include <stdlib.h>
#include <string.h>

static inline int rx_ready(uint32_t base) {
    uint8_t status = reg8_read(base, UART_LINE_STATUS_REG_OFFSET);
    return (status & (1 << UART_LINE_STATUS_DATA_READY_BIT)) != 0;
}

static inline int tx_ready(uint32_t base) {
    uint8_t status = reg8_read(base, UART_LINE_STATUS_REG_OFFSET);
    return (status & (1 << UART_LINE_STATUS_THR_EMPTY_BIT)) != 0;
}

static inline int tx_idle(uint32_t base) {
    uint8_t status = reg8_read(base, UART_LINE_STATUS_REG_OFFSET);
    return (status & (1 << UART_LINE_STATUS_THR_EMPTY_BIT)) &&
           (status & (1 << UART_LINE_STATUS_TMIT_EMPTY_BIT));
}

int uart_open(struct chi_device *device) {
    if (!device || !device->device_addr) {
        return -1;
    }
    // If no config was provided, use the default
    if (!device->cfg) {
        device->cfg = &default_cfg;
    }

    uart_config_t *cfg = (uart_config_t *)device->cfg;

    /* Perform the same initialization you used to do in "uart_init()". */
    uint32_t base = (uint32_t)device->device_addr;
    uint32_t divisor = cfg->clk_freq_hz / (cfg->baud_rate << 4);
    uint8_t dlo = (uint8_t)(divisor);
    uint8_t dhi = (uint8_t)(divisor >> 8);

    // DUMP(base); // 0x03002000
    // DUMP(cfg->clk_freq_hz); // 0x0be6589d
    // DUMP(cfg->baud_rate); // 115200 = 0x0001C200
    // DUMP(divisor); // 108 = 0x0000006C
    // DUMP(dlo); // 108 = 0x6C
    // DUMP(dhi); // 0 = 0x00

    // 1. Disable all interrupts
    reg8_write(base, UART_INTR_ENABLE_REG_OFFSET, 0x00);

    // 2. Enable DLAB, set baud rate
    reg8_write(base, UART_LINE_CONTROL_REG_OFFSET, 0x80);
    reg8_write(base, UART_DLAB_LSB_REG_OFFSET, dlo);
    reg8_write(base, UART_DLAB_MSB_REG_OFFSET, dhi);

    // 3. 8 bits, no parity, one stop bit
    reg8_write(base, UART_LINE_CONTROL_REG_OFFSET, 0x03);

    // 4. Enable & clear FIFO, 14B threshold
    reg8_write(base, UART_FIFO_CONTROL_REG_OFFSET, 0xC7);

    // 5. Autoflow mode
    reg8_write(base, UART_MODEM_CONTROL_REG_OFFSET, 0x20);

    return 0;
}

int uart_close(struct chi_device *device) {
    if (!device || !device->device_addr) {
        return -1;
    }

    uint32_t base = (uint32_t)device->device_addr;

    // 1. Disable all interrupts
    reg8_write(base, UART_INTR_ENABLE_REG_OFFSET, 0x00);

    // 2. Disable DLAB
    reg8_write(base, UART_LINE_CONTROL_REG_OFFSET, 0x00);

    // 3. Reset FIFO CTRL
    reg8_write(base, UART_FIFO_CONTROL_REG_OFFSET, 0x00);

    // 4. Reset modem CTRL
    reg8_write(base, UART_MODEM_CONTROL_REG_OFFSET, 0x00);

    return 0;
}

ssize_t uart_read(struct chi_device *device, void *buffer, uint32_t size, chi_device_callback cb) {
    if (!device || !device->device_addr || !buffer || size == 0) {
        return -1;
    }

    uint8_t *dst = (uint8_t *)buffer;
    uint32_t base = (uint32_t)device->device_addr;

    // Blocking Read
    for (uint32_t i = 0; i < size; i++) {
        while (!rx_ready(base)) {
            // spin
        }
        dst[i] = reg8_read(base, UART_RBR_REG_OFFSET);
    }

    if (cb) {
        (void)cb(device);
    }

    return (ssize_t)size;
}

ssize_t uart_write(struct chi_device *device, const void *buffer, uint32_t size,
                   chi_device_callback cb) {
    if (!device || !device->device_addr || !buffer || size == 0) {
        return -1;
    }

    const uint8_t *src = (const uint8_t *)buffer;
    uint32_t base = (uint32_t)device->device_addr;

    // Blocking Write
    for (uint32_t i = 0; i < size; i++) {
        while (!tx_ready(base)) {
            // spin
        }
        reg8_write(base, UART_THR_REG_OFFSET, src[i]);
    }

    if (cb) {
        (void)cb(device);
    }

    return (ssize_t)size;
}