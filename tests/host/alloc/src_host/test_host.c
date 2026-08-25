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
#include "alloc.h"

// Import HAL Headers

#if defined(TARGET_PLATFORM_CHIMERA_CONVOLVE) && defined(HARDWARE_BACKEND_ASIC)
void setGPIO0_UART() {
    // Connect UART port to GPIO 0 Pad
    padframe_aon_gpio_0_mux_set(CHIMERA_PADFRAME_AON_GPIO_0_group_UART0_port_TX);

    // Set GPIO 0 regs to transmit
    padframe_aon_gpio_0_cfg_rxe_set(0);  // Disable Pad's Receiver
    padframe_aon_gpio_0_cfg_trie_set(0); // Disable the tri-state transmitter
}
#endif

int main(void) {
#if defined(TARGET_PLATFORM_CHIMERA_CONVOLVE) && defined(HARDWARE_BACKEND_ASIC)
    // Connect UART to GPIO 0
    setGPIO0_UART();
#endif

    int8_t ret = 0;

    int8_t *bufferA = (int8_t *)memory_island_malloc(42);
    int8_t *bufferB = (int8_t *)memory_island_malloc(58);
    memory_island_free(bufferA);
    int8_t *bufferC = (int8_t *)memory_island_malloc(21);

    printf("Buffer A: %p\n", bufferA);
    printf("Buffer B: %p\n", bufferB);
    printf("Buffer C: %p\n", bufferC);

    if (bufferA != bufferC) ret = -1;

    return ret;
}
