// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#ifndef HAL_CLINT_H
#define HAL_CLINT_H

#include "interrupt_api.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * \defgroup hal_clint CLINT HAL Interface
 * @ingroup hal_interrupt
 * @brief CLINT-specific implementation of the generic interrupt-controller API.
 * @{
 */

/**
 * @brief Platform-specific mtime type definition.
 *
 * - In a 64-bit system, clint_mtime_t is typedef'd to uint64_t.
 * - In a 32-bit system, clint_mtime_t is a structure with `.low` and `.high` members.
 */
#ifdef PLATFORM_IS_64BIT
/**
 * @brief 64-bit representation of mtime.
 */
typedef uint64_t clint_mtime_t;
#else
/**
 * @brief 32-bit representation of mtime (split into low/high).
 */
typedef struct {
    uint32_t low;  /**< Lower 32 bits of mtime. */
    uint32_t high; /**< Upper 32 bits of mtime. */
} clint_mtime_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Retrieves the current CLINT mtime value.
 *
 * @return The current CLINT timer value.
 */
extern clint_mtime_t clint_get_mtime(void);

/**
 * @brief Compares two CLINT mtime values.
 *
 * @param a First CLINT time value.
 * @param b Second CLINT time value.
 * @return 1 if a is less than b, 0 otherwise.
 */
extern int clint_mtime_less_than(clint_mtime_t a, clint_mtime_t b);

/**
 * @brief Spins (busy-waits) until the specified CLINT mtime value is reached.
 *
 * @param tgt_mtime Target CLINT time value.
 */
extern void clint_spin_until(clint_mtime_t tgt_mtime);

/**
 * @brief Spins (busy-waits) for a given number of ticks.
 *
 * @param ticks Number of clock cycles to wait.
 */
extern void clint_spin_ticks(uint32_t ticks);

/**
 * @brief Estimates the core frequency based on a reference measurement period.
 *
 * The parameter `ref_time_inv` is the inverse of the measurement period.
 *
 * @param ref_freq Reference frequency in Hz.
 * @param ref_time_inv Inverse of the measurement period.
 * @return Estimated core frequency in Hz.
 */
extern uint32_t clint_get_core_freq(uint32_t ref_freq, uint32_t ref_time_inv);

/**
 * @brief Sets the mtimecmp register for a specific timer index.
 *
 * @param timer_idx Timer index to configure.
 * @param value Target mtimecmp value.
 */
extern void clint_set_mtimecmpx(uint32_t timer_idx, clint_mtime_t value);

/**
 * @brief Puts the core to sleep until the specified CLINT mtime value is reached.
 *
 * @param timer_idx Timer index.
 * @param tgt_mtime Target CLINT time value for wakeup.
 */
extern void clint_sleep_until(uint32_t timer_idx, clint_mtime_t tgt_mtime);

/**
 * @brief Puts the core to sleep for a specified number of ticks.
 *
 * @param timer_idx Timer index.
 * @param ticks Number of clock cycles to sleep.
 */
extern void clint_sleep_ticks(uint32_t timer_idx, uint32_t ticks);

#ifdef __cplusplus
}
#endif

/**
 * @brief CLINT’s implementation of the generic interrupt-controller API.
 *
 * Users should use the `chi_interrupt_t` + `interrupt_api` abstraction
 * when driving CLINT.
 */
extern chi_interrupt_api_t clint_api;

/** @} */ // end defgroup hal_clint

#endif // HAL_CLINT_H
