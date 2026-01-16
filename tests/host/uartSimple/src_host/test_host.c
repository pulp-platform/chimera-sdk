// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

// Include Standard Libraries
#include <stdio.h>
#include <string.h>

// Include Application Headers

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "driver.h"

// Include Runtime Headers
#include "util.h"

// Import HAL Headers
#include "interface_api.h"

#if defined(TARGET_PLATFORM_CHIMERA_CONVOLVE) && defined(HARDWARE_BACKEND_ASIC)
void setGPIO0_UART_TX() {
    // Connect UART port to GPIO 0 Pad
    chimera_padframe_aon_gpio_0_mux_set(CHIMERA_PADFRAME_AON_GPIO_0_group_UART0_port_TX);

    // Set GPIO 0 regs to transmit
    chimera_padframe_aon_gpio_0_cfg_rxe_set(0);  // Disable Pad's Receiver
    chimera_padframe_aon_gpio_0_cfg_trie_set(0); // Disable the tri-state transmitter
}
void setGPIO1_UART_RX() {
    // Connect UART port to GPIO 1 Pad
    chimera_padframe_aon_gpio_1_mux_set(CHIMERA_PADFRAME_AON_GPIO_1_group_UART0_port_RX);
}
#endif

int main(void) {
#if defined(TARGET_PLATFORM_CHIMERA_CONVOLVE) && defined(HARDWARE_BACKEND_ASIC)
    // Connect UART TX to GPIO 0
    setGPIO0_UART_TX();
    // Connect UART RX to GPIO 1
    setGPIO1_UART_RX();
#endif

    // 1. Configure the UART from defaults
    uart_config_t uart_cfg = default_uart_cfg;

    // 2. Read the RTC frequency from a hardware register
    uint32_t rtc_freq = *reg32(&__base_regs, CHESHIRE_RTC_FREQ_REG_OFFSET);

    // 3. Calculate the desired core frequency from the RTC frequency
    uint32_t reset_freq = clint_get_core_freq(rtc_freq, 512);

    // 4. Update the UART config with the calculated frequency
    uart_cfg.clk_freq_hz = reset_freq;

    // 5. Initialize the UART interface
    chi_interface_t uart_iface = {
        .base = (uintptr_t)&__base_uart,
        .cfg = &uart_cfg,
        .api = &default_uart_api,
    };

    // 6. Open the UART interface
    if (iface_open(&uart_iface) != 0) {
        return -1;
    }

    // 7. Prepare data to send
    const char uart_cmd[] = "WB";
    size_t cmd_len = sizeof(uart_cmd) - 1;

    // 8. Prepare expected response and buffer
    const char expected_response[] = "WB OK";
    size_t expected_len = sizeof(expected_response) - 1;
    char response_buffer[sizeof(expected_response)] = {0};

    // 9. Write the command to UART
    if (iface_write(&uart_iface, uart_cmd, (uint32_t)cmd_len, NULL) < 0) {
        iface_close(&uart_iface);
        return -1;
    }

    // 10. Read the response from UART
    if (iface_read(&uart_iface, response_buffer, (uint32_t)expected_len, NULL) < 0) {
        iface_close(&uart_iface);
        return -1;
    }

    // 11. Validate the response
    bool ok = (strncmp(response_buffer, expected_response, expected_len) == 0);

    printf("\n");
    printf("UART Response: '%s'\n", response_buffer);
    printf("Expected Response: '%s'\n", expected_response);

    iface_close(&uart_iface);
    return ok ? 0 : -1;
}
