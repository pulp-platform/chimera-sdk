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

// Import HAL Headers

#define CLUSTER 0
#define STACK_ADDRESS (_chimera_clusterBase[CLUSTER] + 0x20000 - 1)

static offloadArgs_t offloadArgs = {.value = 0xdeadbeef};

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

    void *stack_cluster_ptr[NUM_CLUSTER_CORES];
    generate_snitchCluster_SPs_uniform(CLUSTER, (void *)STACK_ADDRESS, 0x2000, stack_cluster_ptr);

    setup_snitchCluster_interruptHandler(clusterInterruptHandler);

    set_snitchCluster_clockGating(CLUSTER, 0);

    set_snitchCluster_reset(CLUSTER, 1);
    for (volatile int i = 0; i < 10; i++);
    set_snitchCluster_reset(CLUSTER, 0);

    printf_log("Waiting for cluster to finish...\n");

    offload_snitchCluster(testReturn, &offloadArgs, stack_cluster_ptr, CLUSTER);
    uint32_t retVal = wait_snitchCluster_return(CLUSTER);

    set_snitchCluster_clockGating(CLUSTER, 1);

    printf("Returned value: 0x%08x (%d)\n", retVal, retVal);
    printf("Expected value: 0x%08x\n", (TESTVAL | 0x000000001));

    return (retVal != (TESTVAL | 0x000000001));
}