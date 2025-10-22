// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Moritz Scherer <scheremo@iis.ee.ethz.ch>

// Include Standard Libraries
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Include Runtime Headers
#include "snrt.h"

void snrt_init() {
    extern char __tbss_start, __tbss_end, __tdata_start, __tdata_end;
    extern volatile uint32_t __cbss_start, __cbss_end, __cdata_start, __cdata_end;
    extern volatile uint32_t __cdata_lma_start, __cdata_lma_end;
    extern char __l1_c4_start, __l1_c4_end;
    extern uint32_t _edram;

    /********** Cluster Memory Initialization **********/
    if (snrt_is_dm_core()) {
        // Preload cdata section from LMA to VMA
        size_t size = (size_t)(&__cdata_end) - (size_t)(&__cdata_start);
        // memcpy((void *)(&__cdata_start), (void *)(&__cdata_lma_start), size);
        snrt_dma_start_1d((void *)(&__cdata_start), (void *)(&__cdata_lma_start), size);

        // Clear the cbss section
        size = (size_t)(&__cbss_end) - (size_t)(&__cbss_start);
        // memset((void *)(&__cbss_start), 0, size);
        snrt_dma_start_1d((void *)(&__cbss_start), (void *)(snrt_zero_memory_ptr()), size);
        snrt_dma_wait_all();
    }

    /********** Thread Local Storage Initialization **********/
    volatile uint32_t tls_ptr;
    asm volatile("mv %0, tp" : "=r"(tls_ptr) : :);

    // Preload the tbss section
    size_t size_tdata = (size_t)(&__tdata_end) - (size_t)(&__tdata_start);
    memcpy((void *)tls_ptr, (void *)(&__tdata_start), size_tdata);
    // snrt_dma_start_1d((void *)tls_ptr, (void *)(&__tdata_start), size_tdata);

    // Clear the tbss section
    size_t size_tbss = (size_t)(&__tbss_end) - (size_t)(&__tbss_start);
    memset((void *)(tls_ptr + size_tdata), 0, size_tbss);
    // snrt_dma_start_1d((void *)(tls_ptr + (size_t)(&__tbss_start) - (size_t)(&__tdata_start)),
    //   (void *)(snrt_zero_memory_ptr()), size_tdata);
    // snrt_dma_wait_all();

    /********** Cluster Initialization **********/
    if (snrt_is_dm_core()) {
        // Initialize the cluster local storage pointer
        _cls_ptr = (cls_t *)&__l1_c4_start;

        // Initialize the L1 allocator
        snrt_l1_allocator()->base = ALIGN_UP((uint32_t)&__l1_c4_start, MIN_CHUNK_SIZE);
        snrt_l1_allocator()->end = snrt_l1_end_addr();
        snrt_l1_allocator()->next = snrt_l1_allocator()->base;

        snrt_l3_allocator()->base = ALIGN_UP((uint32_t)&_edram, MIN_CHUNK_SIZE);
        snrt_l3_allocator()->end = snrt_l3_allocator()->base;
        snrt_l3_allocator()->next = snrt_l3_allocator()->base;

        // Initialize the printf mutex
        snrt_printf_init();
    }

    snrt_cluster_hw_barrier();
}