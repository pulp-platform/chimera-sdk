// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Moritz Scherer <scheremo@iis.ee.ethz.ch>

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

#define __cluster_data __attribute__((__section__(".cdata")))
#define __cluster_bss __attribute__((__section__(".cbss")))

static __cluster_data int8_t cluster_local_var1 = 1;
static __cluster_bss int8_t cluster_local_var2[64];
static __cluster_data int8_t cluster_local_var3 = 3;
static __cluster_data int8_t cluster_local_var4 = 4;

static __thread int32_t thread_local_var1 = 1;
static __thread int32_t thread_local_var2[64];
static __thread int32_t thread_local_var3 = 3;
static __thread int32_t thread_local_var4 = 4;

/**
 * @brief Interrupt handler for the cluster, which clears the interrupt flag for the current hart.
 *
 * @warning Stack, thread and global pointer might not yet be set up!
 */
__attribute__((naked)) void clusterInterruptHandler() {
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

/**
 * @brief Main function of the cluster test.
 *
 * @return int Return 0 if the test was successful, -1 otherwise.
 */
int32_t testReturn(void *args) {
    extern char __tbss_start, __tbss_end, __tdata_start, __tdata_end;
    extern volatile uint32_t __cbss_start, __cbss_end, __cdata_start, __cdata_end;
    extern char __cdata_lma_start, __cdata_lma_end;

    snrt_init();

    if (snrt_is_dm_core()) {
        size_t size_tdata = (size_t)(&__tdata_end) - (size_t)(&__tdata_start);
        size_t size_ctbss = (size_t)(&__tbss_end) - (size_t)(&__tbss_start);
        size_t size_cdata = (size_t)(&__cdata_end) - (size_t)(&__cdata_start);
        size_t size_cbss = (size_t)(&__cbss_end) - (size_t)(&__cbss_start);

        printf("Size of .tbss  : %6d bytes (%p - %p)\n", size_ctbss, &__tbss_start, &__tbss_end);
        printf("Size of .tdata : %6d bytes (%p - %p)\n", size_tdata, &__tdata_start, &__tdata_end);
        printf("Size of .cdata : %6d bytes (%p - %p)\n", size_cdata, &__cdata_start, &__cdata_end);
        printf("Size of .cbss  : %6d bytes (%p - %p)\n", size_cbss, &__cbss_start, &__cbss_end);

        printf("Cluster Local Data @ %p = %#x\n", &cluster_local_var1, cluster_local_var1);
        printf("Cluster Local Data @ %p = %#x\n", &cluster_local_var2, cluster_local_var2[0]);
        printf("Cluster Local Data @ %p = %#x\n", &cluster_local_var3, cluster_local_var3);
        printf("Cluster Local Data @ %p = %#x\n", &cluster_local_var4, cluster_local_var4);
    }

    snrt_cluster_hw_barrier();

    printf("Cluster Local Storage @ %p = %#x\n", &_cls_ptr, cls());

    printf("Var 1 @ %p = %#x\n", &thread_local_var1, thread_local_var1);
    printf("Var 2 @ %p = %#x\n", &thread_local_var2, thread_local_var2[0]);
    printf("Var 3 @ %p = %#x\n", &thread_local_var3, thread_local_var3);
    printf("Var 4 @ %p = %#x\n", &thread_local_var4, thread_local_var4);

    if (snrt_is_dm_core()) {
        printf("L1 Allocator @ %p:\n", snrt_l1_allocator());
        printf("  base @ %p = %#x\n", &snrt_l1_allocator()->base, snrt_l1_allocator()->base);
        printf("  end  @ %p = %#x\n", &snrt_l1_allocator()->end, snrt_l1_allocator()->end);
        printf("  next @ %p = %#x\n", &snrt_l1_allocator()->next, snrt_l1_allocator()->next);

        printf("L3 Allocator @ %p:\n", snrt_l3_allocator());
        printf("  base @ %p = %#x\n", &snrt_l3_allocator()->base, snrt_l3_allocator()->base);
        printf("  end  @ %p = %#x\n", &snrt_l3_allocator()->end, snrt_l3_allocator()->end);
        printf("  next @ %p = %#x\n", &snrt_l3_allocator()->next, snrt_l3_allocator()->next);
    }

    snrt_cluster_hw_barrier();

    if (snrt_is_dm_core()) {
        snrt_dma_start_1d((void *)(&__cdata_start), (void *)(&__cdata_lma_start), 64);
        snrt_dma_start_1d((void *)(&__cbss_start), (void *)(snrt_zero_memory_ptr()), 64);
        snrt_dma_wait_all();
    }

    snrt_cluster_hw_barrier();
    return 0;
}