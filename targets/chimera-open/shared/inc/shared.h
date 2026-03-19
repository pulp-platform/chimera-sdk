// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef CHIMERA_SHARED_H
#define CHIMERA_SHARED_H

#include <stdint.h>

#include "addr_maps/soc_addr_map.h"

/**
 * @defgroup target_shared Shared Communication Interface
 * @ingroup device_snitchCluster
 * @brief Host–device shared memory layout for the chimera-open target.
 *
 * All binaries (host + every cluster device) map the @c .common section to the
 * same physical address (@c 0x48000000).  The host linker script loads
 * @c .common; device linker scripts mark it @c NOLOAD.  The @c packed
 * attribute ensures identical field offsets regardless of ISA or ABI.
 * @{
 */

/**
 * @brief Shared memory region exchanged between the host and all cluster devices.
 *
 * Fields are accessed by both the CVA6 host (RV64IMC) and the Snitch cluster
 * cores (RV32IMAFD) via the AXI interconnect.  @c __attribute__((packed))
 * guarantees identical layout on both ISAs.
 */
typedef struct __attribute__((packed)) {
    volatile uint32_t host_to_device; /**< HTIF channel: host → device semihosting word. */
    volatile uint32_t device_to_host; /**< HTIF channel: device → host semihosting word. */

    /** @brief Flag array: host sets @c host_to_device_flag[i]=1 to start cluster @c i. */
    volatile uint32_t host_to_device_flag[NUM_CLUSTERS];

    /** @brief Flag array: cluster @c i sets @c device_to_host_flag[i]=1 when done. */
    volatile uint32_t device_to_host_flag[NUM_CLUSTERS];

    /** @brief Per-core function pointer written by the host before releasing a core. */
    volatile uint32_t trampoline_function[NUM_CLUSTER_CORES];

    /** @brief Per-core argument pointer written by the host before releasing a core. */
    volatile uint32_t trampoline_args[NUM_CLUSTER_CORES];

    /** @brief Per-core stack pointer written by the host before releasing a core. */
    volatile uint32_t trampoline_stack[NUM_CLUSTER_CORES];

} chimera_shared_data_t;

/** @brief Singleton instance of the shared communication region, placed in @c .common. */
extern chimera_shared_data_t shared_data;

/** @} */ // end defgroup target_shared

#endif /* CHIMERA_SHARED_H */
