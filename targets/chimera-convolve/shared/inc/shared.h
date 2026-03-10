// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

#include "addr_maps/soc_addr_map.h"

/*
 * Shared data structure exchanged between the host (CVA6, RV32IMC) and all
 * device accelerators (Snitch clusters, RV32IMAFD).
 *
 * __attribute__((packed)) guarantees identical field layout regardless of the
 * ABI in use.  All fields use fixed-width types for the same reason.
 *
 * Both the host and each device define a 'shared_data' variable in the
 * '.common' section:
 *   - Host link.ld maps '.common' into memisl at 0x48000000 (loaded).
 *   - Device link.ld maps '.common' into memisl as NOLOAD (same address,
 *     no bytes emitted — device accesses host memory via AXI interconnect).
 */

typedef struct __attribute__((packed)) {
    /*
     * HTIF semihosting channels (replace the legacy
     * shared_data.device_to_host/shared_data.host_to_device symbols previously defined in crt0.S
     * .htif sections). Offset 0 is 64-byte aligned (shared_data itself is aligned(64)).
     */
    volatile uint64_t host_to_device; /* replaces shared_data.device_to_host   (offset  0) */
    volatile uint64_t device_to_host; /* replaces shared_data.host_to_device (offset  8) */

    /* Host sets host_to_device_flag[i] = 1 to signal cluster i to start. */
    volatile uint32_t host_to_device_flag[NUM_CLUSTERS];

    /* Cluster i sets device_to_host_flag[i] = 1 when it is done. */
    volatile uint32_t device_to_host_flag[NUM_CLUSTERS];

    /**
     * @brief Persistent trampoline function pointers for each cluster core.
     *
     * Each entry holds the function to be called by the trampoline on the corresponding core.
     */
    volatile uint32_t trampoline_function[NUM_CLUSTER_CORES];

    /**
     * @brief Persistent argument storage for each cluster core's trampoline function.
     *
     * Each entry holds the `void*` argument passed to the trampoline on the corresponding core.
     */
    volatile uint32_t trampoline_args[NUM_CLUSTER_CORES];

    /**
     * @brief Persistent stack pointer storage for each cluster core's trampoline context.
     *
     * Each entry holds the stack pointer to be loaded by the trampoline on the corresponding core.
     */
    volatile uint32_t trampoline_stack[NUM_CLUSTER_CORES];

} chimera_shared_data_t;

extern chimera_shared_data_t shared_data;
