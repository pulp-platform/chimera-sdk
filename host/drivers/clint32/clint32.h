// SPDX-FileCopyrightText: 2022 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/**
 * \defgroup drivers_clint_32 32-bit CLINT Driver
 * @ingroup drivers_clint
 * @ingroup drivers
 * @brief 32-bit CLINT driver implementation for Chimera-SDK.
 * @{
 *
 * This header provides the declarations for the 32-bit CLINT driver.
 * The generic CLINT HAL API functions (declared in hal/inc/clint.h) are
 * aliased to the 32-bit implementations in this driver.
 */

#pragma once

/* Ensure that we are in 32-bit mode. If PLATFORM_IS_64BIT was defined,
   undefine it so that the HAL selects the 32-bit type definition. */
#ifdef PLATFORM_IS_64BIT
#undef PLATFORM_IS_64BIT
#endif

#include "clint.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/** @} */ // End of drivers_clint_32 group
