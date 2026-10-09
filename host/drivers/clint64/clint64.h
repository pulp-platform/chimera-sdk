// SPDX-FileCopyrightText: 2022 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/**
 * \defgroup drivers_clint_64 64-bit CLINT Driver
 * @ingroup drivers_clint
 * @ingroup drivers
 * @brief 64-bit CLINT driver implementation for Chimera-SDK.
 * @{
 *
 * This header provides the declarations for the 64-bit CLINT driver.
 * The generic CLINT HAL API functions (declared in hal/inc/clint.h) are
 * aliased to the 64-bit implementations in this driver.
 *
 */

#pragma once

/* Define PLATFORM_IS_64BIT to force the HAL type to be uint64_t */
#ifndef PLATFORM_IS_64BIT
#define PLATFORM_IS_64BIT
#endif

#include "clint.h"

#ifdef __cplusplus
extern "C" {
#endif

/* No additional declarations are needed here.
   The strong implementations are provided in clint64.c and
   aliased to the generic HAL API functions. */

#ifdef __cplusplus
}
#endif

/** @} */
