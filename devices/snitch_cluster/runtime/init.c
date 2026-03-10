// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "soc.h"

// Include Runtime Headers
#include "snrt.h"

void snrt_init() {
    /* Linker script symbols: declare as extern char[] and use the symbol
     * address directly (i.e. __sym, not &__sym). Never declare them as
     * object types (e.g. volatile uint32_t) — the linker only provides
     * an address token, not actual storage. */
    extern char __tbss_start[], __tbss_end[], __tdata_start[], __tdata_end[];

    extern char __base_l1_alias[];

    extern char __cbss_start[], __cbss_end[], __cdata_start[], __cdata_end[];
    extern char __cdata_lma_start[], __cdata_lma_end[];
    /* __cbss_lma_start/__cbss_lma_end are metadata-only: the .cbss section
     * is NOLOAD, so no bytes exist at the LMA. The runtime must never use
     * these as a DMA source — zero-init is performed via the hardware
     * zero-memory region instead. */
    extern char __cbss_lma_start[], __cbss_lma_end[];

    /* __l3_heap_start/__l3_heap_end are linker-defined address tokens for
     * the L3 heap region. L1 cluster heap starts are accessed via
     * _chimera_clusterHeapStart[] from soc_addr_map.h. */
    extern char __l3_heap_start[], __l3_heap_end[];

    /********** Thread Local Storage Initialization **********/
    /* Store tp as uintptr_t to avoid truncation on wider address models. */
    uintptr_t tls_ptr;
    asm volatile("mv %0, tp" : "=r"(tls_ptr) : :);

    size_t size_tdata = (uintptr_t)__tdata_end - (uintptr_t)__tdata_start;
    memcpy((void *)tls_ptr, (void *)__tdata_start, size_tdata);

    // Clear the tbss section
    size_t size_tbss = (uintptr_t)__tbss_end - (uintptr_t)__tbss_start;
    uintptr_t tbss_off = (uintptr_t)__tbss_start - (uintptr_t)__tdata_start;
    memset((void *)(tls_ptr + tbss_off), 0, size_tbss);

    /********** Cluster Initialization **********/
    if (snrt_is_dm_core()) {
        uint32_t cluster_idx = snrt_cluster_idx();

        // Preload l1 data from LMA to VMA
        uintptr_t __l1_lma_start = _chimera_clusterL1LmaStart[cluster_idx];
        uintptr_t __l1_lma_end = _chimera_clusterL1LmaEnd[cluster_idx];

        size_t size_l1 = __l1_lma_end - __l1_lma_start;
        if (size_l1) {
            snrt_dma_start_1d((void *)_chimera_clusterL1Start[cluster_idx], (void *)__l1_lma_start,
                              size_l1);
        }

        // Preload cdata section from LMA to VMA
        size_t size_cdata = (uintptr_t)__cdata_end - (uintptr_t)__cdata_start;

        // Translate the CLS alias address to the physical L1 address of this cluster
        uintptr_t __cdata_local_start = (uintptr_t)__cdata_start - (uintptr_t)__base_l1_alias +
                                        (uintptr_t)_chimera_clusterBase[cluster_idx];
        if (size_cdata) {
            snrt_dma_start_1d((void *)__cdata_local_start, (void *)__cdata_lma_start, size_cdata);
        }

        // Clear the cbss section
        size_t size_cbss = (uintptr_t)__cbss_end - (uintptr_t)__cbss_start;
        // Translate the CLS alias address to the physical L1 address of this cluster
        uintptr_t __cbss_local_start = (uintptr_t)__cbss_start - (uintptr_t)__base_l1_alias +
                                       (uintptr_t)_chimera_clusterBase[cluster_idx];
        if (size_cbss) {
            /* snrt_zero_memory_ptr() returns the cluster's hardware zero-memory
             * region — a read-only aperture that always reads as 0. The DMA
             * uses it as a constant-source "memset" to zero the cluster-local
             * BSS without CPU involvement, i.e. it does NOT need a zero-filled
             * source buffer. */
            snrt_dma_start_1d((void *)__cbss_local_start, (void *)snrt_zero_memory_ptr(),
                              size_cbss);
        }

        // Initialize the cluster local storage pointer
        uintptr_t l1_heap_base = _chimera_clusterHeapStart[cluster_idx];
        _cls_ptr = (cls_t *)l1_heap_base;

        // Initialize the L1 allocator
        snrt_l1_allocator()->base = ALIGN_UP(l1_heap_base + sizeof(cls_t), MIN_CHUNK_SIZE);
        snrt_l1_allocator()->end = snrt_l1_end_addr();
        snrt_l1_allocator()->next = snrt_l1_allocator()->base;

        snrt_l3_allocator()->base = ALIGN_UP((uintptr_t)__l3_heap_start, MIN_CHUNK_SIZE);
        snrt_l3_allocator()->end = (uintptr_t)__l3_heap_end;
        snrt_l3_allocator()->next = snrt_l3_allocator()->base;

        snrt_dma_wait_all();

        // Initialize the printf mutex
        snrt_printf_init();

#ifdef TRACE
        printf("Size of .tdata : %6zu bytes (LMA %p - %p, VMA %p - %p)\n", size_tdata,
               (void *)__tdata_start, (void *)__tdata_end, (void *)tls_ptr,
               (void *)(tls_ptr + size_tdata));
        printf("Size of .tbss  : %6zu bytes (LMA %p - %p, VMA %p - %p)\n", size_tbss,
               (void *)__tbss_start, (void *)__tbss_end, (void *)(tls_ptr + tbss_off),
               (void *)(tls_ptr + tbss_off + size_tbss));
        printf("Size of .l1    : %6zu bytes (LMA %p - %p, VMA %p - %p)\n", size_l1,
               (void *)__l1_lma_start, (void *)__l1_lma_end,
               (void *)_chimera_clusterL1Start[cluster_idx],
               (void *)_chimera_clusterL1End[cluster_idx]);
        printf("Size of .cdata : %6zu bytes (LMA %p - %p, VMA %p - %p, aliased at %p - %p)\n",
               size_cdata, (void *)__cdata_lma_start, (void *)__cdata_lma_end,
               (void *)__cdata_local_start, (void *)(__cdata_local_start + size_cdata),
               (void *)__cdata_start, (void *)__cdata_end);
        printf("Size of .cbss  : %6zu bytes (LMA %p - %p, VMA %p - %p, aliased at %p - %p)\n",
               size_cbss, (void *)__cbss_lma_start, (void *)__cbss_lma_end,
               (void *)__cbss_local_start, (void *)(__cbss_local_start + size_cbss),
               (void *)__cbss_start, (void *)__cbss_end);

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

void snrt_exit(int exit_code) {

    uint32_t cluster_idx = snrt_cluster_idx();

    // Write to SOC_CTRL_BASE + cluster_idx
    *reg32((void *)SOC_CTRL_BASE, CHIMERA_SNITCH_CLUSTER_0_RETURN_REG_OFFSET + cluster_idx) =
        exit_code;

    return;
}