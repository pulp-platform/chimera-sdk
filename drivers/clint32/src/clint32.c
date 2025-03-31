// Copyright 2022 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>
//
// This file provides the strong (driver-specific) implementations for the
// CLINT functions using a 32-bit representation (a struct with 'low' and 'high').
// These functions will override the weak HAL symbols.
//

/**
 * \addtogroup drivers
 * @{
 * \defgroup drivers_clint32 32-bit CLINT Driver
 * @{
 * @brief 32-bit CLINT driver implementation for Chimera-SDK.
 *
 *
 */

#include "clint32.h"
#include "regs/clint.h"
#include "util.h"
#include "params.h"

/**
 * @brief Retrieves the current CLINT `mtime` value.
 *
 * This function safely reads the 64-bit timer value from two 32-bit registers,
 * ensuring atomicity by checking that the high register did not change between reads.
 *
 * @return The current CLINT time as a clint_mtime_t structure.
 */
clint_mtime_t clint32_get_mtime(void) {
    clint_mtime_t mtime;
    uint32_t high_check;
    do {
        mtime.high = *reg32(&__base_clint, CLINT_MTIME_HIGH_REG_OFFSET);
        mtime.low = *reg32(&__base_clint, CLINT_MTIME_LOW_REG_OFFSET);
        high_check = *reg32(&__base_clint, CLINT_MTIME_HIGH_REG_OFFSET);
    } while (mtime.high != high_check);
    return mtime;
}

/**
 * @brief Compares two CLINT `mtime` values.
 *
 * @param a First CLINT time value.
 * @param b Second CLINT time value.
 * @return 1 if a < b, 0 otherwise.
 */
int clint32_mtime_less_than(clint_mtime_t a, clint_mtime_t b) {
    return (a.high < b.high) || (a.high == b.high && a.low < b.low);
}

/**
 * @brief Spins (busy-waits) until the specified CLINT `mtime` value is reached.
 *
 * @param tgt_mtime Target CLINT time value.
 */
void clint32_spin_until(clint_mtime_t tgt_mtime) {
    while (clint32_mtime_less_than(clint32_get_mtime(), tgt_mtime)) {
        ; // Busy wait
    }
}

/**
 * @brief Spins (busy-waits) for a given number of ticks.
 *
 * @param ticks Number of clock cycles to wait.
 */
void clint32_spin_ticks(uint32_t ticks) {
    clint_mtime_t start = clint32_get_mtime();
    clint_mtime_t target = {start.low + ticks, start.high};
    if (target.low < start.low) {
        target.high++;
    }
    clint32_spin_until(target);
}

/**
 * @brief Estimates the core frequency based on a reference measurement period.
 *
 * @param ref_freq Reference frequency in Hz.
 * @param ref_time_inv Inverse of the measurement period.
 * @return Estimated core frequency in Hz.
 */
uint32_t clint32_get_core_freq(uint32_t ref_freq, uint32_t ref_time_inv) {
    uint32_t start_mcycle, end_mcycle;
    uint32_t num_ticks = ref_freq / ref_time_inv;
    clint_mtime_t start, end = clint32_get_mtime();

    do {
        start_mcycle = get_mcycle();
        start = clint32_get_mtime();
    } while (start.low == end.low && start.high == end.high);

    do {
        end_mcycle = get_mcycle();
        end = clint32_get_mtime();
    } while (clint32_mtime_less_than(end, (clint_mtime_t){start.low + num_ticks, start.high}));

    return ((end_mcycle - start_mcycle) * ref_freq) / (end.low - start.low);
}

/**
 * @brief Sets the CLINT `mtimecmp` register for a specific timer index.
 *
 * @param timer_idx Timer index to configure.
 * @param value Target `mtimecmp` value.
 */
void clint32_set_mtimecmpx(uint32_t timer_idx, clint_mtime_t value) {
    uint32_t offs = timer_idx << 3;
    *reg32(&__base_clint, CLINT_MTIMECMP_HIGH0_REG_OFFSET + offs) = value.high;
    *reg32(&__base_clint, CLINT_MTIMECMP_LOW0_REG_OFFSET + offs) = value.low;
}

/**
 * @brief Puts the core into sleep mode until the specified CLINT `mtime` value is reached.
 *
 * Before sleeping, this function programs the mtimecmp register and enables interrupts.
 *
 * @param timer_idx Timer index.
 * @param tgt_mtime Target CLINT time value.
 */
void clint32_sleep_until(uint32_t timer_idx, clint_mtime_t tgt_mtime) {
    if (clint32_mtime_less_than(clint32_get_mtime(), tgt_mtime)) return;
    clint32_set_mtimecmpx(timer_idx, tgt_mtime);
    fence();
    set_mtie(1);
    set_mie(1);
    wfi();
}

/**
 * @brief Puts the core into sleep mode for a specified number of ticks.
 *
 * @param timer_idx Timer index.
 * @param ticks Number of clock cycles to sleep.
 */
void clint32_sleep_ticks(uint32_t timer_idx, uint32_t ticks) {
    clint_mtime_t start = clint32_get_mtime();
    clint_mtime_t target = {start.low + ticks, start.high};
    if (target.low < start.low) {
        target.high++;
    }
    clint32_sleep_until(timer_idx, target);
}

// VIVIANEP: Need to skip doxygen generation for these functions
// to avoid duplicated defintion errors in the generated documentation

/// @cond DOXYGEN_SHOULD_SKIP_THIS
extern clint_mtime_t clint_get_mtime()
    __attribute__((alias("clint32_get_mtime"), used, visibility("default")));
extern int clint_mtime_less_than(clint_mtime_t a, clint_mtime_t b)
    __attribute__((alias("clint32_mtime_less_than"), used, visibility("default")));
extern void clint_spin_until(clint_mtime_t tgt_mtime)
    __attribute__((alias("clint32_spin_until"), used, visibility("default")));
extern void clint_spin_ticks(uint32_t ticks)
    __attribute__((alias("clint32_spin_ticks"), used, visibility("default")));
extern uint32_t clint_get_core_freq(uint32_t ref_freq, uint32_t ref_time_inv)
    __attribute__((alias("clint32_get_core_freq"), used, visibility("default")));
extern void clint_set_mtimecmpx(uint32_t timer_idx, clint_mtime_t value)
    __attribute__((alias("clint32_set_mtimecmpx"), used, visibility("default")));
extern void clint_sleep_until(uint32_t timer_idx, clint_mtime_t tgt_mtime)
    __attribute__((alias("clint32_sleep_until"), used, visibility("default")));
extern void clint_sleep_ticks(uint32_t timer_idx, uint32_t ticks)
    __attribute__((alias("clint32_sleep_ticks"), used, visibility("default")));
/// @endcond

/** @} */ // End of drivers_clint32 group
/** @} */ // End of drivers group