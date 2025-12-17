// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include Application Headers

// Include Target Specific Headers
#include "soc.h"
#include "test.h"

// Include Driver Headers
#include "driver.h"
#include "sw/device/lib/dif/dif_gpio.h"

// Include Runtime Headers
#include "log.h"
#include "util.h"

// Import HAL Headers
#include "interface_api.h"

#define TESTVAL (uint32_t)0x1234ABCD

static const dif_gpio_t gpio = {
    .base_addr = (volatile void *)&__base_gpio,
};

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
void setGPIO2_GPIO() {
    // Connect GPIO 2 Pad to GPIO 2
    chimera_padframe_aon_gpio_2_mux_set(CHIMERA_PADFRAME_AON_GPIO_2_group_GPIOA_port_GPIO2);

    // Set GPIO 2 regs to transmit
    chimera_padframe_aon_gpio_2_cfg_rxe_set(0);  // Disable Pad's Receiver
    chimera_padframe_aon_gpio_2_cfg_trie_set(0); // Disable the tri-state transmitter
}
#endif

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

    /**** Register configuration Test */
    // Read all bitfields in hyperbus cfg regs
    printf_log("Reading HyperRAM CFG Registers before configuration...\n");
    for (uint32_t regOff = 0; regOff < (HYPERBUS_PARAM_NUM_BASE_REGS * 4); regOff += 4) {
        volatile uint32_t regVal = *reg32((void *)HYPERBUS_CTRL_BASE, regOff);
        printf_log("  CFG Reg Offset 0x%02X : Value 0x%08X\n", regOff, regVal);
    }

    printf_log("Configuring HyperRAM...\n");
    hyperram_cfg_t hyperram_cfg = hyperram_cfg_default(HYPERRAM_BASE_ADDR, 0x10000000);

    if (hyperram_configure((void *)HYPERBUS_CTRL_BASE, &hyperram_cfg) != 0) {
        printf_log("Error: HyperRAM configuration failed\n");
        return -1;
    }

    // Read all bitfields in hyperbus cfg regs
    for (uint32_t regOff = 0; regOff < (HYPERBUS_PARAM_NUM_BASE_REGS * 4); regOff += 4) {
        volatile uint32_t regVal = *reg32((void *)HYPERBUS_CTRL_BASE, regOff);
        printf_log("  CFG Reg Offset 0x%02X : Value 0x%08X\n", regOff, regVal);
    }

    /****  Simple HyperRAM Read/Write Test  ****/
    volatile uint32_t *hyperMemPtr = (volatile uint32_t *)HYPERRAM_BASE_ADDR;
    volatile uint32_t val;

    // Sweep the delay lines to find a working point
    for (uint32_t delay = 0; delay < 16; delay++) {
        // Set RX delay
        hyperbus_set_t_rx_clk_delay((void *)HYPERBUS_CTRL_BASE, delay);

        // write
        *(hyperMemPtr) = TESTVAL;
    }

    for (uint32_t delay = 0; delay < 16; delay++) {
        // Set TX delay
        hyperbus_set_t_tx_clk_delay((void *)HYPERBUS_CTRL_BASE, delay);

        // read
        val = *(hyperMemPtr);
    }

    // verify
    if (val == TESTVAL) {
        return 0;
    } else {
        return 1;
    }

    return 0;
}