// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
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
#include "clint.h"
#include "uart.h"

#if defined(TARGET_PLATFORM_CHIMERA_CONVOLVE) && defined(HARDWARE_BACKEND_ASIC)
void setGPIO0_UART() {
    // Connect UART port to GPIO 0 Pad
    chimera_padframe_aon_gpio_0_mux_set(CHIMERA_PADFRAME_AON_GPIO_0_group_UART0_port_TX);

    // Set GPIO 0 regs to transmit
    chimera_padframe_aon_gpio_0_cfg_rxe_set(0);  // Disable Pad's Receiver
    chimera_padframe_aon_gpio_0_cfg_trie_set(0); // Disable the tri-state transmitter
}
#endif

int main(void) {
#if defined(TARGET_PLATFORM_CHIMERA_CONVOLVE) && defined(HARDWARE_BACKEND_ASIC)
    // Connect UART to GPIO 0
    setGPIO0_UART();
#endif

    volatile uint32_t *scratch =
        (volatile uint32_t *)(&__base_regs + CHESHIRE_SCRATCH_0_REG_OFFSET);
    uint32_t core_mV = scratch[0];  // e.g. 900
    uint32_t freq_MHz = scratch[1]; // e.g. 170

    printf("@dutctl:dutmeas:fllmhz:%d\r\n", freq_MHz);
    printf("@dutctl:dutmeas:coremv:%d\r\n", core_mV);
    printf("@dutctl:psuctl:%d.%d:0:E36312A_1:1\r\n", core_mV / 1000, core_mV % 1000);

    return 0;
}
