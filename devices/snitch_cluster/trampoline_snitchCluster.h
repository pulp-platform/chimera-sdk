// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _CLUSTER_SNITCH_TRAMPOLINE_INCLUDE_GUARD_
#define _CLUSTER_SNITCH_TRAMPOLINE_INCLUDE_GUARD_

#include <stdint.h>

/** \addtogroup device_snitchCluster
 *  @{
 */

/**
 * @brief Setup the core to a known state.
 *
 * This function will set up the global pointer and thread pointer for the core.
 */
#define _SETUP_GP() \
    asm volatile(".option push\n" \
                 ".option norelax\n" \
                 "la gp, __global_pointer$\n" \
                 ".option pop\n" \
                 : /* No outputs */ \
                 : /* No inputs */ \
                 : /* No clobbered registers */)

/** @} */

#endif //_CLUSTER_SNITCH_TRAMPOLINE_INCLUDE_GUARD_
