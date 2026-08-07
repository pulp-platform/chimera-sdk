// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include Application Headers
#include "test_snitchCluster_iLayerNorm_L1_device1_symbols.h"
#include "test_host.h"

// Include Target Specific Headers
#include "soc.h"
#include "test.h"

// Include Driver Headers
#include "driver.h"
#include "sw/device/lib/dif/dif_gpio.h"

// Include Runtime Headers
#include "log.h"
#include "fll.h"
#include "util.h"

// Import HAL Headers
#include "interface_api.h"

#define STACK_SIZE_CC 0x1000
#define STACK_SIZE_DM 0x1000

#define STACK_ADDRESS_0 (_chimera_clusterBase[0] + 0x20000 - 1)
static uint32_t stack_size_0[CLUSTER_0_NUMCORES] = {STACK_SIZE_CC, STACK_SIZE_DM};

#define STACK_ADDRESS_1 (_chimera_clusterBase[1] + 0x20000 - 1)
static uint32_t stack_size_1[CLUSTER_1_NUMCORES] = {STACK_SIZE_CC, STACK_SIZE_DM};

#define STACK_ADDRESS_2 (_chimera_clusterBase[2] + 0x20000 - 1)
static uint32_t stack_size_2[CLUSTER_2_NUMCORES] = {STACK_SIZE_CC, STACK_SIZE_DM};

#define STACK_ADDRESS_3 (_chimera_clusterBase[3] + 0x20000 - 1)
static uint32_t stack_size_3[CLUSTER_3_NUMCORES] = {STACK_SIZE_CC, STACK_SIZE_DM};

#define STACK_ADDRESS_4 (_chimera_clusterBase[4] + 0x20000 - 1)

static uint32_t stack_size_4[CLUSTER_4_NUMCORES] = {STACK_SIZE_CC, STACK_SIZE_CC, STACK_SIZE_CC,
                                                    STACK_SIZE_CC, STACK_SIZE_CC, STACK_SIZE_CC,
                                                    STACK_SIZE_CC, STACK_SIZE_CC, STACK_SIZE_DM};

// Timeout for cluster execution (in RTC ticks)
#define CLUSTER_TIMEOUT_MS 10000

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

    test_cluster_result_t test_result = {0};

    test_cluster_args_t test_args = {
        .repetitions = 1,
        .result = &test_result,
        .args = NULL,
    };

    test_cluster_cfg_t test_cfg = {
        .name = "L1 iLayerNorm Test",
        .mode = TEST_MODE_AUTOMATIC,
        .default_frequency_mhz = 200, // Frequency in MHz in automatic mode
        .default_repetitions = 1,     // Number of repetitions in automatic mode
        .timeout = CLUSTER_TIMEOUT_MS,
        .clusters = 1,
        .clusterIds = {4},
        .stack_start = {(void *)STACK_ADDRESS_4},
        .stack_sizes = {stack_size_4},
        .function_test = (void *)device1_ilayernorm_l1_test,
        .function_trampoline = (void *)device1_trampoline,
        .function_interrupt = (void *)device1_clusterInterruptHandler,
        .args = {&test_args},
    };

    /*
     * Check SCRATCH0 register to override test mode
     * - 0: AUTOMATIC
     * - 1: DUTCTL
     * - 2: INTERACTIVE
     */
    volatile uint32_t *scratch =
        (volatile uint32_t *)(&__base_regs + CHESHIRE_SCRATCH_0_REG_OFFSET);

    switch (scratch[0]) {
    case TEST_MODE_DUTCTL:
        test_cfg.mode = TEST_MODE_DUTCTL;
        break;
    case TEST_MODE_INTERACTIVE:
        test_cfg.mode = TEST_MODE_INTERACTIVE;
        break;
    case TEST_MODE_AUTOMATIC:
    default:
        test_cfg.mode = TEST_MODE_AUTOMATIC;
        break;
    }

    return test_cluster(&test_cfg);
}