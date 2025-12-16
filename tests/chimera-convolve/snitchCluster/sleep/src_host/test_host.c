// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include Application Headers
#include "test_cluster.h"
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
#include "clint.h"
#include "util.h"

// Import HAL Headers
#include "interface_api.h"

#define CLUSTER 4
#define STACK_ADDRESS (_chimera_clusterBase[CLUSTER] + 0x20000 - 1)

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

    char input_buffer[32];
    test_mode_t mode = TEST_MODE_INTERACTIVE;

    volatile uint32_t *scratch =
        (volatile uint32_t *)(&__base_regs + CHESHIRE_SCRATCH_0_REG_OFFSET);

    if (scratch[0] == 1) {
        mode = TEST_MODE_DUTCTL;
    }

    uint32_t rtc_freq = *reg32(&__base_regs, CHESHIRE_RTC_FREQ_REG_OFFSET);

#if defined(HARDWARE_BACKEND_ASIC)
    // Read the RTC frequency from a hardware register
    restore_default_freq(rtc_freq);

    // Calculate the initial core frequency from the RTC frequency
    // Relative Error = 1 / 128 = 0.78%
    // Absolute Error = 78.125 kHz
    uint32_t ref_time_inv = rtc_freq / 128; // 32768 // 128 = 256
    uint32_t core_freq = clint_get_core_freq(rtc_freq, ref_time_inv);
#else
    uint32_t core_freq = 500000000; // 500 MHz for RTL
#endif

    if (mode != TEST_MODE_DUTCTL) {
        printf_log("========================================\n");
        printf_log("Chimera Sleep Test with FLL Configuration\n");
        printf_log("========================================\n");
        printf_log("Initial frequency: %u.%03u MHz\n", (core_freq / 1000000),
                   (core_freq % 1000000));
    }

    if (mode == TEST_MODE_DUTCTL) {
        uint32_t core_mV = scratch[2]; // e.g. 900
        printf("@dutctl:dutmeas:cfg_core_mV:%d\r\n", core_mV);
        printf("@dutctl:psuctl:%d.%d:0:E36312A_1:1\r\n", core_mV / 1000, core_mV % 1000);
    }

#if defined(HARDWARE_BACKEND_ASIC)
    int target_freq_mhz = 0;
    if (mode == TEST_MODE_DUTCTL) {
        // In DUTCTL mode, read target frequency from scratch register
        target_freq_mhz = scratch[3]; // e.g. 170
        printf("@dutctl:dutmeas:cfg_fll_MHz:%d\r\n", target_freq_mhz);
    } else {
        // Ask user for target frequency
        printf_log("Enter target frequency in MHz (10-1000, or -1 to skip FLL configuration): ");
        fflush(stdout);

        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            printf("\n");
            printf_log("Error reading input\n");
            return -1;
        }

        target_freq_mhz = atoi(input_buffer);
        printf("%d\n", target_freq_mhz);
    }

    uint32_t actual_freq = core_freq;

    if (target_freq_mhz == -1) {
        printf_log("Skipping FLL configuration, using default frequency\n");
    } else if (target_freq_mhz < 10 || target_freq_mhz > 1000) {
        printf_log("Error: Frequency out of range (10-1000 MHz)\n");
        return -1;
    } else {
        if (mode != TEST_MODE_DUTCTL) {
            printf_log("Configuring system to %d MHz...\n", target_freq_mhz);
        }

        uint32_t target_freq = target_freq_mhz * 1000000;
        actual_freq = configure_fll(target_freq, rtc_freq);
        if (actual_freq == 0) {
            printf_log("Error: Failed to configure FLL\n");
            return -1;
        }

        if (mode == TEST_MODE_DUTCTL) {
            printf("@dutctl:dutmeas:meas_fll_MHz:%d.%03d\r\n", (actual_freq / 1000000),
                   (actual_freq % 1000000));
        } else {
            printf_log("FLL configured successfully!\n");
            printf_log("Actual frequency: %u.%03u MHz\n", (actual_freq / 1000000),
                       (actual_freq % 1000000));
        }
    }

#else
    uint32_t actual_freq = core_freq;
    printf_log("Note: FLL configuration only available on ASIC target\n");
#endif

    void *stack_cluster_ptr[NUM_CLUSTER_CORES];
    generate_snitchCluster_SPs_uniform(CLUSTER, (void *)STACK_ADDRESS, 0x2000, stack_cluster_ptr);

    setup_snitchCluster_interruptHandler(clusterInterruptHandler);

    set_snitchCluster_clockGating(CLUSTER, 0);

    set_snitchCluster_reset(CLUSTER, 1);
    for (volatile int i = 0; i < 10; i++);
    set_snitchCluster_reset(CLUSTER, 0);

    offload_snitchCluster(wfi_test, NULL, stack_cluster_ptr, CLUSTER);

    if (mode == TEST_MODE_DUTCTL) {
        // For 500 iterations, computation should last at least 1s even at 400MHz
        printf("@dutctl:psumeas:meas_core_V:500:E36312A_1:1\r\n");
    }

#ifdef HARDWARE_BACKEND_RTL
    clint_sleep_ticks(0, 20);
#else
    // Sleep for 2 seconds
    clint_sleep_ticks(0, 2 * rtc_freq);
#endif

    if (mode == TEST_MODE_DUTCTL) {
        printf("@dutctl:dutmeas:meas_errors:0\n");
    } else {
        printf_log("Test completed successfully!\n");
    }

    return 0;
}