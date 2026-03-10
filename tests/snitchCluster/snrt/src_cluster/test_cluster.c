// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>
#include <string.h>

// Include Application Headers
#include "test_cluster.h"
#include "test_host.h"

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "trampoline_snitchCluster.h"

// Include Runtime Headers
#include "snrt.h"

SNRT_CLUSTER_L1_COPY(static volatile int8_t cluster_local_var1) = 1;
SNRT_CLUSTER_L1_ZERO(static volatile int cluster_local_var2[64]);

static __thread int32_t thread_local_var1 = 1;
static __thread int32_t thread_local_var2[64];

SNRT_CLUSTER_L1(static int32_t cluster0_private_var1, 0) = 0x1;
SNRT_CLUSTER_L1(static int32_t cluster0_private_var2[8], 0);

SNRT_CLUSTER_L1(static int32_t cluster1_private_var1, 1) = 0x11;
SNRT_CLUSTER_L1(static int32_t cluster1_private_var2[16], 1);

SNRT_CLUSTER_L1(static int32_t cluster2_private_var1, 2) = 0x21;
SNRT_CLUSTER_L1(static int32_t cluster2_private_var2[24], 2);

SNRT_CLUSTER_L1(static int32_t cluster3_private_var1, 3) = 0x31;
SNRT_CLUSTER_L1(static int32_t cluster3_private_var2[32], 3);

SNRT_CLUSTER_L1(static int32_t cluster4_private_var1, 4) = 0x41;
SNRT_CLUSTER_L1(static int32_t cluster4_private_var2[40], 4);

/**
 * @brief Interrupt handler for the cluster, which clears the interrupt flag for the current hart.
 *
 * @warning Stack, thread and global pointer might not yet be set up!
 */
__attribute__((naked)) void clusterInterruptHandler() {
    _SET_CLUSTER_BUSY();
    _SETUP_GP();

    asm volatile(
        // Load mhartid CSR into t0
        "csrr t0, mhartid\n"

        // Load clint base address into t1
        "la t1, __base_clint\n"

        // Calculate the interrupt target address: t1 = t1 + (t0 * 4)
        "slli t0, t0, 2\n"
        "add t1, t1, t0\n"
        // Store 0 to the interrupt target address
        "sw zero, 0(t1)\n"
        "ret"
        :            // No outputs
        :            // No inputs
        : "t0", "t1" // Declare clobbered registers
    );
}

void clusterDefaultHandler() {
    *reg32(&__base_clint, CLINT_MSIP_REG_OFFSET) = 0;

    while (1) {
    };
}

/**
 * @brief Main function of the cluster test.
 *
 * @return int Return 0 if the test was successful, -1 otherwise.
 */
int32_t testReturn(void *args) {

    *reg32((void *)SOC_CTRL_BASE, CHIMERA_SNITCH_INTR_HANDLER_ADDR_REG_OFFSET) =
        (uint32_t)clusterDefaultHandler;

    extern char __tbss_start[], __tbss_end[], __tdata_start[], __tdata_end[];
    extern char __cbss_start[], __cbss_end[], __cdata_start[], __cdata_end[];
    extern char __cdata_lma_start[], __cdata_lma_end[];

    snrt_init();

    if (snrt_is_dm_core()) {
        printf("Cluster Local Data 1 @ %p = %#x\n", &cluster_local_var1, cluster_local_var1);
        printf("Cluster Local Data 2 @ %p = %#x\n", &cluster_local_var2, cluster_local_var2[0]);
        cluster_local_var2[0] = snrt_cluster_idx();
        printf("Cluster Local Data 2 @ %p = %#x\n", &cluster_local_var2, cluster_local_var2[0]);

        if (snrt_cluster_idx() == 0) {
            printf("Cluster 0 Private Data 1 @ %p = %#x\n", &cluster0_private_var1,
                   cluster0_private_var1);
            printf("Cluster 0 Private Data 2 @ %p = %#x\n", &cluster0_private_var2,
                   cluster0_private_var2[0]);
            cluster0_private_var2[0] = snrt_cluster_idx();
            printf("Cluster 0 Private Data 2 @ %p = %#x\n", &cluster0_private_var2,
                   cluster0_private_var2[0]);
        } else if (snrt_cluster_idx() == 1) {
            printf("Cluster 1 Private Data 1 @ %p = %#x\n", &cluster1_private_var1,
                   cluster1_private_var1);
            printf("Cluster 1 Private Data 2 @ %p = %#x\n", &cluster1_private_var2,
                   cluster1_private_var2[0]);
            cluster1_private_var2[0] = snrt_cluster_idx();
            printf("Cluster 1 Private Data 2 @ %p = %#x\n", &cluster1_private_var2,
                   cluster1_private_var2[0]);
        } else if (snrt_cluster_idx() == 2) {
            printf("Cluster 2 Private Data 1 @ %p = %#x\n", &cluster2_private_var1,
                   cluster2_private_var1);
            printf("Cluster 2 Private Data 2 @ %p = %#x\n", &cluster2_private_var2,
                   cluster2_private_var2[0]);
            cluster2_private_var2[0] = snrt_cluster_idx();
            printf("Cluster 2 Private Data 2 @ %p = %#x\n", &cluster2_private_var2,
                   cluster2_private_var2[0]);
        } else if (snrt_cluster_idx() == 3) {
            printf("Cluster 3 Private Data 1 @ %p = %#x\n", &cluster3_private_var1,
                   cluster3_private_var1);
            printf("Cluster 3 Private Data 2 @ %p = %#x\n", &cluster3_private_var2,
                   cluster3_private_var2[0]);
            cluster3_private_var2[0] = snrt_cluster_idx();
            printf("Cluster 3 Private Data 2 @ %p = %#x\n", &cluster3_private_var2,
                   cluster3_private_var2[0]);
        } else if (snrt_cluster_idx() == 4) {
            printf("Cluster 4 Private Data 1 @ %p = %#x\n", &cluster4_private_var1,
                   cluster4_private_var1);
            printf("Cluster 4 Private Data 2 @ %p = %#x\n", &cluster4_private_var2,
                   cluster4_private_var2[0]);
            cluster4_private_var2[0] = snrt_cluster_idx();
            printf("Cluster 4 Private Data 2 @ %p = %#x\n", &cluster4_private_var2,
                   cluster4_private_var2[0]);
        }
    }

    snrt_cluster_hw_barrier();

    printf("Core %d Local Data 1 @ %p = %#x\n", snrt_cluster_core_idx(), &thread_local_var1,
           thread_local_var1);
    printf("Core %d Local Data 2 @ %p = %#x\n", snrt_cluster_core_idx(), &thread_local_var2,
           thread_local_var2[0]);
    thread_local_var2[0] = snrt_cluster_core_idx();
    printf("Core %d Local Data 2 @ %p = %#x\n", snrt_cluster_core_idx(), &thread_local_var2,
           thread_local_var2[0]);

    snrt_cluster_hw_barrier();
    return 0;
}
