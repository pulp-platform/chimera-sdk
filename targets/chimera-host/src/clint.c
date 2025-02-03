// Copyright 2022 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Nicole Narr <narrn@student.ethz.ch>
// Christopher Reinwardt <creinwar@student.ethz.ch>
// Paul Scheffler <paulsc@iis.ee.ethz.ch>

/**
 * \addtogroup targets_chimera_host_clint
 * @ingroup targets_chimera_host
 * @{
 *
 * @file clint.c
 * @brief CLINT (Core-Local Interruptor) driver implementation for Chimera-Host (32-bit).
 *
 * This file provides implementations for CLINT timer functionality, including
 * timekeeping, timer comparisons, and sleep mechanisms.
 *
 * @author Viviene Potocnik vivianep@iis.ee.ethz.ch
 * @date 2025-01-31
 */

#include "clint.h"
#include "regs/clint.h"
#include "util.h"
#include "params.h"

/**
 * @brief Reads the current CLINT `mtime` value atomically.
 *
 * This function ensures a safe read of the 64-bit timer value by checking
 * the upper register twice.
 *
 * @return The current CLINT `mtime` as a `clint_mtime_t` structure.
 */
clint_mtime_t clint_get_mtime() {
    clint_mtime_t mtime;
    uint32_t high_check;

    // Read the 64-bit timer value safely
    do {
        mtime.high = *reg32(&__base_clint, CLINT_MTIME_HIGH_REG_OFFSET);
        mtime.low = *reg32(&__base_clint, CLINT_MTIME_LOW_REG_OFFSET);
        high_check = *reg32(&__base_clint, CLINT_MTIME_HIGH_REG_OFFSET);
    } while (mtime.high != high_check); // Ensure atomicity

    return mtime;
}

/**
 * @brief Compares two `mtime` values.
 *
 * Since direct 64-bit arithmetic is unavailable, this function ensures safe
 * comparisons using high and low parts separately.
 *
 * @param a First `mtime` value.
 * @param b Second `mtime` value.
 * @return 1 if `a < b`, 0 otherwise.
 */
int clint_mtime_less_than(clint_mtime_t a, clint_mtime_t b) {
    return (a.high < b.high) || (a.high == b.high && a.low < b.low);
}

/**
 * @brief Spins (busy-waits) until the specified `mtime` value is reached.
 *
 * @param tgt_mtime Target `mtime` value.
 */
void clint_spin_until(clint_mtime_t tgt_mtime) {
    while (clint_mtime_less_than(clint_get_mtime(), tgt_mtime));
}

/**
 * @brief Spins for a given number of clock ticks.
 *
 * @param ticks Number of ticks to wait.
 */
void clint_spin_ticks(uint32_t ticks) {
    clint_mtime_t start_mtime = clint_get_mtime();
    clint_mtime_t tgt_mtime = {start_mtime.low + ticks, start_mtime.high};

    // Handle carry in case of 32-bit overflow
    if (tgt_mtime.low < start_mtime.low) tgt_mtime.high++;

    clint_spin_until(tgt_mtime);
}

/**
 * @brief Estimates the core frequency based on reference frequency and timing.
 *
 * This function measures the number of cycles elapsed between CLINT timer ticks
 * and calculates the core frequency.
 *
 * @param ref_freq Reference frequency in Hz.
 * @param ref_time_inv Inverse of the measurement period.
 * @return Estimated core frequency in Hz.
 */
uint32_t clint_get_core_freq(uint32_t ref_freq, uint32_t ref_time_inv) {
    uint32_t end_mcycle, start_mcycle;
    uint32_t num_ticks = ref_freq / ref_time_inv;
    clint_mtime_t end_mtime = clint_get_mtime(), start_mtime;

    // Capture start times until RTC tick rolls over
    do {
        start_mcycle = get_mcycle();
        start_mtime = clint_get_mtime();
    } while (start_mtime.low == end_mtime.low && start_mtime.high == end_mtime.high);

    // Capture end times until reaching target ticks
    do {
        end_mcycle = get_mcycle();
        end_mtime = clint_get_mtime();
    } while (clint_mtime_less_than(end_mtime,
                                   (clint_mtime_t){start_mtime.low + num_ticks, start_mtime.high}));

    return (end_mcycle - start_mcycle) * ref_freq / (end_mtime.low - start_mtime.low);
}

/**
 * @brief Sets the CLINT `mtimecmp` register for a given timer index.
 *
 * @param timer_idx Timer index to configure.
 * @param value Target `mtimecmp` value.
 */
void clint_set_mtimecmpx(uint32_t timer_idx, clint_mtime_t value) {
    uint32_t mtimecmp_offs = timer_idx << 3;

    // Write high register first
    *reg32(&__base_clint, CLINT_MTIMECMP_HIGH0_REG_OFFSET + mtimecmp_offs) = value.high;
    *reg32(&__base_clint, CLINT_MTIMECMP_LOW0_REG_OFFSET + mtimecmp_offs) = value.low;
}

/**
 * @brief Puts the core into sleep mode until the specified `mtime` value is reached.
 *
 * @param timer_idx Timer index.
 * @param tgt_mtime Target `mtime` value.
 */
void clint_sleep_until(uint32_t timer_idx, clint_mtime_t tgt_mtime) {
    if (clint_mtime_less_than(clint_get_mtime(), tgt_mtime)) return;

    // Set comparison register to `tgt_mtime`
    clint_set_mtimecmpx(timer_idx, tgt_mtime);
    fence();

    // Enable timer and global interrupts
    set_mtie(1);
    set_mie(1);

    // Wait for interrupt
    wfi();
}

/**
 * @brief Puts the core into sleep mode for a given number of ticks.
 *
 * @param timer_idx Timer index.
 * @param ticks Number of ticks to sleep.
 */
void clint_sleep_ticks(uint32_t timer_idx, uint32_t ticks) {
    clint_mtime_t start_mtime = clint_get_mtime();
    clint_mtime_t tgt_mtime = {start_mtime.low + ticks, start_mtime.high};

    // Handle carry
    if (tgt_mtime.low < start_mtime.low) tgt_mtime.high++;

    clint_sleep_until(timer_idx, tgt_mtime);
}

/** @} */ // End of targets_chimera_host_clint group
