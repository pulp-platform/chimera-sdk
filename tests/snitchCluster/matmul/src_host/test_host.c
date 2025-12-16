// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries

// Include Application Headers
#include "test_cluster.h"
#include "test_host.h"

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "driver.h"

// Include Runtime Headers
#include "log.h"
#include "util.h"
#include "clint.h"

// Import HAL Headers

#define CLUSTER1 4
#define STACK_ADDRESS (_chimera_clusterBase[CLUSTER1] + 0x20000 - 1)

extern uintptr_t volatile tohost, fromhost;

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

    setup_snitchCluster_interruptHandler(clusterInterruptHandler);

    set_snitchCluster_clockGating(CLUSTER1, 0);

    set_snitchCluster_reset(CLUSTER1, 1);
    for (volatile int i = 0; i < 10; i++);
    set_snitchCluster_reset(CLUSTER1, 0);

    printf_log("Waiting for cluster to finish...\n");

    argCluster_t arg_struct = {0};
    argCluster_t *arg = &arg_struct;

    // Disable interrupts
    set_mie(0);
    offload_snitchCluster(testReturn, (void *)arg, stack_cluster_ptr, CLUSTER1);

    // Enable interrupts
    set_mie(1);

    // Handle tohost/fromhost communication
    while (snitchCluster_busy(CLUSTER1)) {
        // Wait for tohost to be set by the device
        if (tohost != 0) {
            volatile uint32_t syscall_addr = tohost;

            // Acknowledge tohost
            tohost = 0;

            // printf("Host received tohost: %#x\n", tohost);

            // Cluster does tohost = (uintptr_t)buf->hdr.syscall_mem;
            uint32_t *syscall_mem = (uint32_t *)syscall_addr;

            // printf("Host handling syscall %u: fd=%#x, buf=%p, len=%#x\n", syscall_mem[0],
            //        syscall_mem[1], (void *)syscall_mem[2], syscall_mem[3]);
            if (syscall_mem[0] == 64) { // sys_write
                fwrite((const void *)syscall_mem[2], 1, syscall_mem[3], (FILE *)syscall_mem[1]);
                fflush((FILE *)syscall_mem[1]);
            } else {
                printf_log("Unknown syscall: %u\n", syscall_mem[0]);
            }

            // Notify cluster that syscall is done
            fromhost = syscall_addr;
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
