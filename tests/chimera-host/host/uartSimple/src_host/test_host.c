// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#include "params.h"
#include "clint.h"
#include "uart.h"
#include "util.h"
#include "regs/cheshire.h"
#include "regs/soc_ctrl.h"

#include <stdio.h>
#include <string.h>

int main(void) {
    // 1. Configure the UART from defaults
    uart_config_t uart_cfg = default_cfg;

    // 2. Read the RTC frequency from a hardware register
    uint32_t rtc_freq = *reg32(&__base_regs, CHESHIRE_RTC_FREQ_REG_OFFSET);

    // 3. Calculate the desired core frequency from the RTC frequency
    uint32_t reset_freq = clint_get_core_freq(rtc_freq, 2500);

    // 4. Update the UART config with the calculated frequency
    uart_cfg.clk_freq_hz = reset_freq;

    // 5. Initialize the UART interface
    chi_interface_t uart_iface = {
        .base = (uintptr_t)&__base_uart,
        .cfg = &uart_cfg,
        .api = &uart_api,
    };

    // 6. Open the UART interface
    if (uart_open(&uart_iface) != 0) {
        return -1;
    }

    // 7. Prepare data to send
    const char uart_cmd[] = "WB";
    size_t cmd_len = sizeof(uart_cmd) - 1;

    // 8. Prepare expected response and a buffer to read into
    const char expected_response[] = "WB OK";
    size_t expected_len = sizeof(expected_response) - 1;
    char response_buffer[sizeof(expected_response)] = {0};

    // 9. Write the command to UART
    if (uart_write(&uart_iface, uart_cmd, (uint32_t)cmd_len, NULL) < 0) {
        uart_close(&uart_iface);
        return -1;
    }

    // 10. Read the response from UART
    if (uart_read(&uart_iface, response_buffer, (uint32_t)expected_len, NULL) < 0) {
        uart_close(&uart_iface);
        return -1;
    }

    // 11. Validate the response
    bool ok = (strncmp(response_buffer, expected_response, expected_len) == 0);
    uart_close(&uart_iface);
    return ok ? 0 : -1;
}
