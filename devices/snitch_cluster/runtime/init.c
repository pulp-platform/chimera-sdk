// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Include Runtime Headers
#include "snrt.h"

void snrt_init() {
    extern volatile uint32_t __tbss_start, __tbss_end, __tdata_start, __tdata_end;

    extern volatile uint32_t __base_l1_alias;

    extern volatile uint32_t __cbss_start, __cbss_end, __cdata_start, __cdata_end;
    extern volatile uint32_t __cdata_lma_start, __cdata_lma_end, __cbss_lma_start, __cbss_lma_end;

    extern volatile uint32_t __l1_c0_heap_start, __l1_c1_heap_start, __l1_c2_heap_start,
        __l1_c3_heap_start, __l1_c4_heap_start;
    extern volatile uint32_t __l3_heap_start, __l3_heap_end;

    /********** Thread Local Storage Initialization **********/
    volatile uint32_t tls_ptr;
    asm volatile("mv %0, tp" : "=r"(tls_ptr) : :);

    // Preload the tbss section
    size_t size_tdata = (size_t)(&__tdata_end) - (size_t)(&__tdata_start);
    memcpy((void *)tls_ptr, (void *)(&__tdata_start), size_tdata);

    // Clear the tbss section
    size_t size_tbss = (size_t)(&__tbss_end) - (size_t)(&__tbss_start);
    memset((void *)(tls_ptr + size_tdata), 0, size_tbss);

    /********** Cluster Initialization **********/
    if (snrt_is_dm_core()) {
        uint32_t cluster_idx = snrt_cluster_idx();

        // Preload l1 data from LMA to VMA
        uint32_t __l1_lma_start = _chimera_clusterL1LmaStart[cluster_idx];
        uint32_t __l1_lma_end = _chimera_clusterL1LmaEnd[cluster_idx];

        size_t size_l1 = __l1_lma_end - __l1_lma_start;
        if (size_l1) {
            snrt_dma_start_1d((void *)_chimera_clusterL1Start[cluster_idx], (void *)__l1_lma_start,
                              size_l1);
        }

        // Preload cdata section from LMA to VMA
        size_t size_cdata = (size_t)(&__cdata_end) - (size_t)(&__cdata_start);

        // Calculate the start address in L1 in gloabal address space
        uint32_t __cdata_local_start = (uint32_t)&__cdata_start - (uint32_t)&__base_l1_alias +
                                       _chimera_clusterBase[cluster_idx];
        if (size_cdata) {
            snrt_dma_start_1d((void *)__cdata_local_start, (void *)(&__cdata_lma_start),
                              size_cdata);
        }

        // Clear the cbss section
        size_t size_cbss = (size_t)(&__cbss_end) - (size_t)(&__cbss_start);
        // Calculate the start address in L1 in global address space
        uint32_t __cbss_local_start = (uint32_t)&__cbss_start - (uint32_t)&__base_l1_alias +
                                      _chimera_clusterBase[cluster_idx];
        if (size_cbss) {
            snrt_dma_start_1d((void *)__cbss_local_start, (void *)(snrt_zero_memory_ptr()),
                              size_cbss);
        }

        // Initialize the cluster local storage pointer
        uint32_t l1_heap_base = _chimera_clusterHeapStart[cluster_idx];
        _cls_ptr = (cls_t *)l1_heap_base;

        // Initialize the L1 allocator
        snrt_l1_allocator()->base = ALIGN_UP(l1_heap_base + sizeof(cls_t), MIN_CHUNK_SIZE);
        snrt_l1_allocator()->end = snrt_l1_end_addr();
        snrt_l1_allocator()->next = snrt_l1_allocator()->base;

        snrt_l3_allocator()->base = ALIGN_UP((uint32_t)&__l3_heap_start, MIN_CHUNK_SIZE);
        snrt_l3_allocator()->end = (uint32_t)&__l3_heap_end;
        snrt_l3_allocator()->next = snrt_l3_allocator()->base;

        snrt_dma_wait_all();

        // Initialize the printf mutex
        snrt_printf_init();

#ifdef TRACE
        printf("Size of .tdata : %6d bytes (LMA %p - %p, VMA %p - %p)\n", size_tdata,
               &__tdata_start, &__tdata_end, tls_ptr, tls_ptr + size_tdata);
        printf("Size of .tbss  : %6d bytes (LMA %p - %p, VMA %p - %p)\n", size_tbss, &__tbss_start,
               &__tbss_end, tls_ptr + size_tdata, tls_ptr + size_tdata + size_tbss);
        printf("Size of .l1    : %6d bytes (LMA %p - %p, VMA %p - %p)\n", size_l1,
               (void *)__l1_lma_start, (void *)__l1_lma_end,
               (void *)_chimera_clusterL1Start[cluster_idx],
               (void *)_chimera_clusterL1End[cluster_idx]);
        printf("Size of .cdata : %6d bytes (LMA %p - %p, VMA %p - %p, aliased at %p - %p)\n",
               size_cdata, (void *)&__cdata_lma_start, (void *)&__cdata_lma_end,
               (void *)__cdata_local_start, (void *)__cdata_local_start + size_cdata,
               &__cdata_start, &__cdata_end);
        printf("Size of .cbss  : %6d bytes (LMA %p - %p, VMA %p - %p, aliased at %p - %p)\n",
               size_cbss, (void *)&__cbss_lma_start, (void *)&__cbss_lma_end,
               (void *)__cbss_local_start, (void *)__cbss_local_start + size_cbss, &__cbss_start,
               &__cbss_end);

        printf("L1 Allocator @ %p:\n", snrt_l1_allocator());
        printf("  base @ %p = %#x\n", &snrt_l1_allocator()->base, snrt_l1_allocator()->base);
        printf("  end  @ %p = %#x\n", &snrt_l1_allocator()->end, snrt_l1_allocator()->end);
        printf("  next @ %p = %#x\n", &snrt_l1_allocator()->next, snrt_l1_allocator()->next);

        printf("L3 Allocator @ %p:\n", snrt_l3_allocator());
        printf("  base @ %p = %#x\n", &snrt_l3_allocator()->base, snrt_l3_allocator()->base);
        printf("  end  @ %p = %#x\n", &snrt_l3_allocator()->end, snrt_l3_allocator()->end);
        printf("  next @ %p = %#x\n", &snrt_l3_allocator()->next, snrt_l3_allocator()->next);

        printf("Cluster Local Storage @ %p = %#x\n", &_cls_ptr, cls());
#endif
    }

    snrt_cluster_hw_barrier();
}