// Copyright 2022 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

/**
 * \addtogroup targets_chimera_host_clint
 * @ingroup targets_chimera_host
 * @brief CLINT (Core-Local Interruptor) interface for Chimera-Host.
 *
 * This module provides functions for interacting with the CLINT timer,
 * including reading `mtime`, comparing timestamps, and implementing sleep functions.
 *
 * @{
 *
 * @file clint.h
 * @author Nicole Narr
 * @author Christopher Reinwardt
 * @author Paul Scheffler
 * @author Viviane Potocnik
 * @date 2025-01-31
 * @license Apache-2.0
 */

#pragma once

#include <stdint.h>

/**
 * @brief Represents a 64-bit CLINT timer value.
 *
 * This structure holds the low and high 32-bit parts of the CLINT `mtime` counter.
 */
typedef struct {
    uint32_t low;  /**< Lower 32 bits of the `mtime` register. */
    uint32_t high; /**< Upper 32 bits of the `mtime` register. */
} clint_mtime_t;

/**
 * @brief Retrieves the current CLINT `mtime` value.
 *
 * @return The current CLINT time as a `clint_mtime_t` structure.
 */
clint_mtime_t clint_get_mtime();

/**
 * @brief Compares two CLINT `mtime` values.
 *
 * @param a First `mtime` value.
 * @param b Second `mtime` value.
 * @return 1 if `a < b`, 0 otherwise.
 */
int clint_mtime_less_than(clint_mtime_t a, clint_mtime_t b);

/**
 * @brief Spins (busy-waits) until the specified CLINT `mtime` value is reached.
 *
 * @param tgt_mtime Target CLINT time to spin until.
 */
void clint_spin_until(clint_mtime_t tgt_mtime);

/**
 * @brief Spins (busy-waits) for a given number of clock ticks.
 *
 * @param ticks Number of clock cycles to wait.
 */
void clint_spin_ticks(uint32_t ticks);

/**
 * @brief Estimates the core frequency based on a reference measurement period.
 *
 * @note This function assumes a stable clock; `ref_time_inv` should be the
 *       *inverse* of the measurement period.
 *
 * @param ref_freq Reference frequency in Hz.
 * @param ref_time_inv Inverse of the measurement period.
 * @return Estimated core frequency in Hz.
 */
uint32_t clint_get_core_freq(uint32_t ref_freq, uint32_t ref_time_inv);

/**
 * @brief Sets the CLINT `mtimecmp` register for a specific timer index.
 *
 * @param timer_idx Timer index to configure.
 * @param value Target `mtimecmp` value.
 */
void clint_set_mtimecmpx(uint32_t timer_idx, clint_mtime_t value);

/**
 * @brief Puts the core into sleep mode until the specified `mtime` value is reached.
 *
 * @pre Requires an appropriate trap handler to catch the timer interrupt.
 *
 * @param timer_idx Timer index.
 * @param tgt_mtime Target CLINT `mtime` value for waking up.
 */
void clint_sleep_until(uint32_t timer_idx, clint_mtime_t tgt_mtime);

/**
 * @brief Puts the core into sleep mode for a specified number of ticks.
 *
 * @pre Requires an appropriate trap handler to catch the timer interrupt.
 *
 * @param timer_idx Timer index.
 * @param ticks Number of clock cycles to sleep.
 */
void clint_sleep_ticks(uint32_t timer_idx, uint32_t ticks);

/** @} */ // End of targets_chimera_host_clint group