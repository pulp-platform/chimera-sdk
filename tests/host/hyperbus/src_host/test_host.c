// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include Application Headers

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "driver.h"
#include "sw/device/lib/dif/dif_gpio.h"

// Include Runtime Headers
#include "log.h"
#include "util.h"

// Import HAL Headers
#include "interface_api.h"

#define TESTVAL (uint32_t)0x1234ABCD

#if defined(TARGET_PLATFORM_CHIMERA_CONVOLVE) && defined(HARDWARE_BACKEND_ASIC)
static const dif_gpio_t gpio = {
    .base_addr = (volatile void *)&__base_gpio,
};
void setGPIO0_UART_TX() {
    // Connect UART port to GPIO 0 Pad
    padframe_aon_gpio_0_mux_set(CHIMERA_PADFRAME_AON_GPIO_0_group_UART0_port_TX);

    // Set GPIO 0 regs to transmit
    padframe_aon_gpio_0_cfg_rxe_set(0);  // Disable Pad's Receiver
    padframe_aon_gpio_0_cfg_trie_set(0); // Disable the tri-state transmitter
}
void setGPIO1_UART_RX() {
    // Connect UART port to GPIO 1 Pad
    padframe_aon_gpio_1_mux_set(CHIMERA_PADFRAME_AON_GPIO_1_group_UART0_port_RX);
}
void setGPIO2_GPIO() {
    // Connect GPIO 2 Pad to GPIO 2
    padframe_aon_gpio_2_mux_set(CHIMERA_PADFRAME_AON_GPIO_2_group_GPIOA_port_GPIO2);

    // Set GPIO 2 regs to transmit
    padframe_aon_gpio_2_cfg_rxe_set(0);  // Disable Pad's Receiver
    padframe_aon_gpio_2_cfg_trie_set(0); // Disable the tri-state transmitter
}
#endif

uint32_t test_hyperbus(void *base, uint32_t *addr_start, uint32_t size) {
    hyperram_cfg_t hyperram_cfg;
    hyperram_cfg_default(&hyperram_cfg, (uint32_t)(uintptr_t)addr_start, size);

    hyperram_cfg_write(base, &hyperram_cfg);
    hyperram_cfg_print(base);

    uint32_t val = 0;

    for (uint32_t delay = 0; delay < (HYPERBUS_T_TX_CLK_DELAY_MASK + 1); delay++) {
        // Set delay lines
        hyperbus_set_t_tx_clk_delay(base, delay);
        hyperbus_set_t_rx_clk_delay(base, delay);

        // write
        *(addr_start) = TESTVAL;
        fence();

        // read
        val = *(addr_start);
        fence();
    }

    return (val != TESTVAL);
}

int main(void) {
#if defined(TARGET_PLATFORM_CHIMERA_CONVOLVE) && defined(HARDWARE_BACKEND_ASIC)
    // Set GPIO 2 to output and enable FLL bypass
    setGPIO2_GPIO();
    // Connect UART to GPIO 0
    setGPIO0_UART_TX();
    // Connect UART RX to GPIO 1
    setGPIO1_UART_RX();

    dif_result_t result = dif_gpio_output_set_enabled(&gpio, 2, kDifToggleEnabled);
    if (result != kDifOk) {
        printf_log("Error: Cannot set GPIO 2 as output\n");
        return -1;
    }
#endif

    printf("HyperBus Configuration\n");
    hyperram_cfg_print((void *)HYPERBUS_CTRL_BASE);

    printf("Starting HyperBus tests...\n");
    uint32_t test_result =
        test_hyperbus((void *)HYPERBUS_CTRL_BASE, (void *)HYPERRAM_BASE_ADDR, 0x10000);

    return test_result;
}