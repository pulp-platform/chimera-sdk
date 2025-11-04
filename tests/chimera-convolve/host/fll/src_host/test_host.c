// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

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

void setGPIO0_UART() {
    // Connect UART port to GPIO 0 Pad
    chimera_padframe_aon_gpio_0_mux_set(CHIMERA_PADFRAME_AON_GPIO_0_group_UART0_port_TX);

    // Set GPIO 0 regs to transmit
    chimera_padframe_aon_gpio_0_cfg_rxe_set(0);  // Disable Pad's Receiver
    chimera_padframe_aon_gpio_0_cfg_trie_set(0); // Disable the tri-state transmitter
}

void setGPIO0_GPIO() {
    // Connect GPIO 0 Pad to GPIO 0
    chimera_padframe_aon_gpio_0_mux_set(CHIMERA_PADFRAME_AON_GPIO_0_group_GPIOA_port_GPIO0);

    // Set GPIO 0 regs to transmit
    chimera_padframe_aon_gpio_0_cfg_rxe_set(0);  // Disable Pad's Receiver
    chimera_padframe_aon_gpio_0_cfg_trie_set(0); // Disable the tri-state transmitter
}

void setGPIO2_GPIO() {
    // Connect GPIO 2 Pad to GPIO 2
    chimera_padframe_aon_gpio_2_mux_set(CHIMERA_PADFRAME_AON_GPIO_2_group_GPIOA_port_GPIO2);

    // Set GPIO 2 regs to transmit
    chimera_padframe_aon_gpio_2_cfg_rxe_set(0);  // Disable Pad's Receiver
    chimera_padframe_aon_gpio_2_cfg_trie_set(0); // Disable the tri-state transmitter
}

void gpio_masked_bit_write(void *base_addr, uint32_t reg_lower_offset, uint32_t reg_upper_offset,
                           uint32_t index, bool val) {
    int offset = (index < 16) ? reg_lower_offset : reg_upper_offset;
    uint32_t mask = 1 << (index % 16);
    // Write value to the masked register
    *reg32(base_addr, offset) = (mask << 16) | (val ? mask : 0u);
}

void gpioWrite(uint32_t index, int value) {
    gpio_masked_bit_write(&__base_gpio, GPIO_MASKED_OUT_LOWER_REG_OFFSET,
                          GPIO_MASKED_OUT_UPPER_REG_OFFSET, index, value);
}

void gpioSetOutput(uint32_t index, int value) {
    gpio_masked_bit_write(&__base_gpio, GPIO_MASKED_OE_LOWER_REG_OFFSET,
                          GPIO_MASKED_OE_UPPER_REG_OFFSET, index, value);
}

int main(void) {
    // Use default UART configuration
    uart_config_t uart_cfg = default_uart_cfg;

    // Connect UART to GPIO 0
    setGPIO0_UART();
    // Connect GPIO 2 to GPIO 2
    setGPIO2_GPIO();

    // Set GPIO 2 and GPIO 3 to output
    gpioSetOutput(2, 1);
    gpioSetOutput(3, 1);

    gpioWrite(2, 1);

    // Read the RTC frequency from a hardware register
    uint32_t rtc_freq = *reg32(&__base_regs, CHESHIRE_RTC_FREQ_REG_OFFSET);

    // Calculate the desired core frequency from the RTC frequency
    uint32_t core_freq = clint_get_core_freq(rtc_freq, 512);

    printf("Chimera running at %d.%d MHz!\n", (core_freq / 1000000), (core_freq % 1000000));

    // Configure FLLs to 200 MHz
    gpioWrite(2, 0);
    volatile uint32_t *fllPtr = getFllPtr(0);
    initFll(fllPtr);
    setFllFreq(fllPtr, 24480, 3);

    fllPtr = getFllPtr(1);
    initFll(fllPtr);
    setFllFreq(fllPtr, 24480, 3);

    // Delay for FLL lock
    for (volatile int i = 0; i < 1000; i++);

    // Disable FLL bypass
    gpioWrite(2, 0);

    // Calculate the desired core frequency from the RTC frequency
    uint32_t core_freq_fll = clint_get_core_freq(rtc_freq, 16384);

    // Update the UART config with the calculated frequency
    uart_cfg.clk_freq_hz = core_freq_fll;

    // Initialize the UART interface
    default_uart_inst.cfg = &uart_cfg;

    // Re-Open the UART interface
    if (iface_open(&default_uart_inst) != 0) {
        return -1;
    }

    printf("Chimera running at %d.%d MHz!\n", (core_freq / 1000000), (core_freq % 1000000));
    printf("Chimera running at %d.%d MHz!\n", (core_freq_fll / 1000000), (core_freq_fll % 1000000));

    setGPIO0_GPIO();

    // Enable FLL bypass
    gpioWrite(2, 1);

    return 0;
}
