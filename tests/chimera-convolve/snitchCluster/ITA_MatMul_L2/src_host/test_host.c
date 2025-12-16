// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include Application Headers
#include "test_cluster.h"
#include "test_host.h"
#include "ITA_dims.h"

// Include Target Specific Headers
#include "soc.h"
#include "test.h"

// Include Driver Headers
#include "driver.h"
#include "sw/device/lib/dif/dif_gpio.h"

// Include Runtime Headers
#include "alloc.h"
#include "log.h"
#include "fll.h"
#include "util.h"

// Import HAL Headers
#include "interface_api.h"

#define CLUSTER 4
#define STACK_ADDRESS (_chimera_clusterBase[CLUSTER] + 0x20000 - 1)
static uint32_t stack_size[CLUSTER_4_NUMCORES] = {0x1000, 0x1000, 0x1000, 0x1000, 0x1000,
                                                  0x1000, 0x1000, 0x1000, 0x4000};

// Timeout for cluster execution (in RTC ticks)
#define CLUSTER_TIMEOUT_MS 10000

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

    test_cluster_result_t test_result = {0};

    ita_matmul_l2_test_args_t args = {0};

    args.output_L2 =
        (int8_t *)memory_island_malloc(SEQUENCE_LENGTH * PROJECTION_SPACE * sizeof(int8_t));

    test_cluster_args_t test_args = {
        .repetitions = 1,
        .result = &test_result,
        .args = (void *)&args,
    };

    test_cluster_cfg_t test_cfg = {
        .name = "L2 ITA MatMul Test",
        .mode = TEST_MODE_AUTOMATIC,
        .default_frequency_mhz = 200, // Frequency in MHz in automatic mode
        .default_repetitions = 1,     // Number of repetitions in automatic mode
        .timeout = CLUSTER_TIMEOUT_MS,
        .clusterId = CLUSTER,
        .stack_start = (void *)STACK_ADDRESS,
        .stack_sizes = stack_size,
        .function_test = (void *)ita_matmul_l2_test,
        .function_interrupt = (void *)clusterInterruptHandler,
        .args = &test_args,
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