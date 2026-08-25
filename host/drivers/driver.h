// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/**
 * @file driver.h
 * @brief Unified driver include header for Chimera-SDK.
 *
 * This header conditionally includes individual peripheral driver headers
 * based on preprocessor flags set by the CMake build system (via
 * DRIVER_MAPPINGS in the target's CMakeLists.txt).
 *
 * Define the relevant @c CHIMERA_DRIVER_* flag before including this header
 * to pull in the corresponding driver API:
 *
 * | Flag                        | Driver included              |
 * |-----------------------------|------------------------------|
 * | @c CHIMERA_DRIVER_CLUSTER   | offload_snitchCluster.h      |
 * | @c CHIMERA_DRIVER_FLL       | fll/fll.h                    |
 * | @c CHIMERA_DRIVER_UART_APB  | uart_apb/uart_apb.h          |
 * | @c CHIMERA_DRIVER_CLINT32   | clint32/clint32.h            |
 * | @c CHIMERA_DRIVER_CLINT64   | clint64/clint64.h            |
 * | @c CHIMERA_DRIVER_HYPERBUS  | hyperbus/hyperbus.h          |
 */

#ifndef _DRIVER_INCLUDE_GUARD_
#define _DRIVER_INCLUDE_GUARD_

#ifdef CHIMERA_DRIVER_CLUSTER
#include "cluster/offload_snitchCluster.h"
#endif

#ifdef CHIMERA_DRIVER_FLL
#include "fll/fll.h"
#endif

#ifdef CHIMERA_DRIVER_UART_APB
#include "uart_apb/uart_apb.h"
#endif

#ifdef CHIMERA_DRIVER_CLINT32
#include "clint32/clint32.h"
#endif

#ifdef CHIMERA_DRIVER_CLINT64
#include "clint64/clint64.h"
#endif

#ifdef CHIMERA_DRIVER_HYPERBUS
#include "hyperbus/hyperbus.h"
#endif

#endif //_DRIVER_INCLUDE_GUARD_
