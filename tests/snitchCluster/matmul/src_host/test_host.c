// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries

// Include Application Headers
#include "test_host.h"

#include "test_snitchCluster_matmul_device1_symbols.h"

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "driver.h"

// Include Runtime Headers
#include "log.h"
#include "util.h"
#include "clint.h"
#include "shared.h"

// Import HAL Headers

#define CLUSTER1 4
#define STACK_ADDRESS (_chimera_clusterBase[CLUSTER1] + 0x20000 - 1)

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

#if defined(HARDWARE_BACKEND_RTL)
    uint32_t core_freq = 500000000; // 500 MHz for RTL
#else
    // 2. Read the RTC frequency from a hardware register
    uint32_t rtc_freq = *reg32(&__base_regs, CHESHIRE_RTC_FREQ_REG_OFFSET);

    // 3. Calculate the desired core frequency from the RTC frequency with 1.56% error
    uint32_t core_freq = clint_get_core_freq(rtc_freq, 512);

    printf("Chimera running at %d.%d MHz!\n", (core_freq / 1000000), (core_freq % 1000000));
#endif

    void *stack_cluster_ptr[NUM_CLUSTER_CORES];
    generate_snitchCluster_SPs_uniform(CLUSTER1, (void *)STACK_ADDRESS, 0x2000, stack_cluster_ptr);

    setup_snitchCluster_interruptHandler(device1_clusterInterruptHandler);

    set_snitchCluster_clockGating(CLUSTER1, 0);

    set_snitchCluster_reset(CLUSTER1, 1);
    for (volatile int i = 0; i < 10; i++);
    set_snitchCluster_reset(CLUSTER1, 0);

    printf_log("Waiting for cluster to finish...\n");

    argCluster_t arg_struct = {0};
    argCluster_t *arg = &arg_struct;

    // Disable interrupts
    set_mie(0);
    offload_snitchCluster(device1_testReturn, device1_trampoline, (void *)arg, stack_cluster_ptr,
                          CLUSTER1);

    // Enable interrupts
    set_mie(1);

    // Handle shared_data.device_to_host/shared_data.host_to_device communication
    while (snitchCluster_busy(CLUSTER1)) {
        // Wait for shared_data.device_to_host to be set by the device
        if (shared_data.device_to_host != 0) {
            volatile uint32_t syscall_addr = shared_data.device_to_host;

            // Acknowledge shared_data.device_to_host
            shared_data.device_to_host = 0;

            // Cluster does shared_data.device_to_host = (uintptr_t)buf->hdr.syscall_mem;
            uint32_t *syscall_mem = (uint32_t *)((uint64_t)syscall_addr);
            const void *syscall_buf = (const void *)((uint64_t)syscall_mem[2]);

            if (syscall_mem[0] == 64) { // sys_write
                fwrite(syscall_buf, 1, syscall_mem[3], stdout);
                fflush(stdout);
            } else {
                printf_log("Unknown syscall: %u\n", syscall_mem[0]);
            }

            // Notify cluster that syscall is done
            shared_data.host_to_device = syscall_addr;
        }

        // Enter low-power mode until next interrupt
        clint_sleep_ticks(0, 10);
    }

    uint32_t retVal = wait_snitchCluster_return(CLUSTER1);
    retVal = retVal >> 1;

    set_snitchCluster_clockGating(CLUSTER1, 1);

    printf_log("Returned from cluster: 0x%08x (%d)\n", retVal, retVal);
    uint32_t ops_per_sec = (arg->ops_per_cycle / 1000) * (core_freq / 1000);

    printf("Op/Cycle: %u.%06u\n", arg->ops_per_cycle / 1000000, arg->ops_per_cycle % 1000000);
    printf("Op/s: %u.%03u M\n", ops_per_sec / 1000000, ops_per_sec % 1000000);

    return retVal;
}
