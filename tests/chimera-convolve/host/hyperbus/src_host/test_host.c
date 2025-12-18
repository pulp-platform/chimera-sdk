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
#include "fll.h"

// Import HAL Headers
#include "interface_api.h"

#define TESTVAL (uint32_t)0xFFFFFFFF

static const dif_gpio_t gpio = {
    .base_addr = (volatile void *)&__base_gpio,
};

#if defined(TARGET_PLATFORM_CHIMERA_CONVOLVE) && defined(HARDWARE_BACKEND_ASIC)
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

void test_hyperbus(void *base, uint32_t addr_start, uint32_t size) {
    hyperram_cfg_t hyperram_cfg;
    hyperram_cfg_default(&hyperram_cfg, addr_start, size);

    hyperram_cfg_write(base, &hyperram_cfg);
    hyperram_cfg_print(base);

    volatile uint32_t *ptr = (volatile uint32_t *)addr_start;

    for (uint32_t delay = 0; delay < (HYPERBUS_T_TX_CLK_DELAY_MASK + 1); delay++) {
        // Set delay lines
        hyperbus_set_t_tx_clk_delay(base, delay);
        hyperbus_set_t_rx_clk_delay(base, delay);

        // write
        *(ptr) = TESTVAL;

        // Wait for a bit
        for (volatile int i = 0; i < 10000; i++);
    }
}

int main(void) {
#if defined(TARGET_PLATFORM_CHIMERA_CONVOLVE) && defined(HARDWARE_BACKEND_ASIC)
    // Set GPIO 2 to output and enable FLL bypass
    setGPIO2_GPIO();
    // Connect UART to GPIO 0
    setGPIO0_UART_TX();
    // Connect UART RX to GPIO 1
    setGPIO1_UART_RX();

    // Configure GPIO 2 as output
    dif_result_t result = dif_gpio_output_set_enabled(&gpio, 2, kDifToggleEnabled);
    if (result != kDifOk) {
        printf_log("Error: Cannot set GPIO 2 as output\n");
        return -1;
    }

    // Set GPIO 2 high to enable FLL bypass
    result = dif_gpio_write(&gpio, 2, kDifToggleEnabled);
    if (result != kDifOk) {
        printf_log("Error: Cannot enable FLL bypass\n");
        return 0;
    }
#endif

    // Configure FLL
    // Read the RTC frequency from a hardware register
    uint32_t rtc_freq = *reg32(&__base_regs, CHESHIRE_RTC_FREQ_REG_OFFSET);
    uint32_t target_freq = 50000000; // 50 MHz

    uint32_t actual_freq = configure_fll(target_freq, rtc_freq);

    if (actual_freq == 0) {
        printf("Error: Failed to configure FLL\n");
        return -1;
    }
    printf("FLL configured to %u.%03u MHz\n", (actual_freq / 1000000), (actual_freq % 1000000));

    /**** Register configuration Test */

    // Set drive strength of all hyperbus pads to max

    printf("Drive Strength Configuration\n");
    printf("  CS0 : %d\n", padframe_hyperbus_cs0_cfg_drv_get());
    printf("  CS1 : %d\n", padframe_hyperbus_cs1_cfg_drv_get());
    printf("  CLK : %d\n", padframe_hyperbus_clk_cfg_drv_get());
    printf("  CLKN: %d\n", padframe_hyperbus_clkn_cfg_drv_get());
    printf("  RST : %d\n", padframe_hyperbus_rst_cfg_drv_get());
    printf("  RWDS: %d\n", padframe_hyperbus_rwds_cfg_drv_get());
    printf("  DQ0 : %d\n", padframe_hyperbus_dq0_cfg_drv_get());
    printf("  DQ1 : %d\n", padframe_hyperbus_dq1_cfg_drv_get());
    printf("  DQ2 : %d\n", padframe_hyperbus_dq2_cfg_drv_get());
    printf("  DQ3 : %d\n", padframe_hyperbus_dq3_cfg_drv_get());
    printf("  DQ4 : %d\n", padframe_hyperbus_dq4_cfg_drv_get());
    printf("  DQ5 : %d\n", padframe_hyperbus_dq5_cfg_drv_get());
    printf("  DQ6 : %d\n", padframe_hyperbus_dq6_cfg_drv_get());
    printf("  DQ7 : %d\n", padframe_hyperbus_dq7_cfg_drv_get());

    printf("HyperBus Configuration\n");
    hyperram_cfg_print((void *)HYPERBUS_CTRL_BASE);

    printf("\n");
    printf("Starting HyperBus tests...\n");
    test_hyperbus((void *)HYPERBUS_CTRL_BASE, HYPERBUS_ADDR_START, 0x10000);

    printf("\n");
    printf("\n");

    restore_default_freq(rtc_freq);

    return 0;
}