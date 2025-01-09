// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Philip Wiese <wiesep@iis.ee.ethz.ch>

#ifndef _CLUSTER_SNITCH_TRAMPOLINE_INCLUDE_GUARD_
#define _CLUSTER_SNITCH_TRAMPOLINE_INCLUDE_GUARD_

#include <stdint.h>

/** \addtogroup devices_snitchCluster
 *  @{
 */

/**
 * @brief Setup the core to a known state.
 *
 * This function will set up the global pointer and thread pointer for the core.
 */
#define _SETUP_GP_TP() \
    asm volatile(".option push\n" \
                 ".option norelax\n" \
                 "la gp, __global_pointer$\n" \
                 ".option pop\n" \
                 "mv tp, zero\n" \
                 : /* No outputs */ \
                 : /* No inputs */ \
                 : /* No clobbered registers */)

/** @} */

#endif //_CLUSTER_SNITCH_TRAMPOLINE_INCLUDE_GUARD_
