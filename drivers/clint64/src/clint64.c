// Copyright 2022 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Authors: Nicole Narr <narrn@student.ethz.ch>
//          Christopher Reinwardt <creinwar@student.ethz.ch>
//          Paul Scheffler <paulsc@iis.ee.ethz.ch>
//          Viviane Potocnik <vivianep@iis.ee.ethz.ch>
//

/**
 * \addtogroup drivers
 * @{
 * \defgroup drivers_clint64 64-bit CLINT Driver
 * @{
 * @brief 64-bit CLINT driver implementation for Chimera-SDK.
 *
 *
 */

#include "clint64.h"
#include "regs/clint.h"
#include "util.h"
#include "params.h"

/**
 * @brief Retrieves the current CLINT `mtime` value.
 *
 * This function reads the high and low 64-bit registers and combines them into
 * a 64-bit value.
 *
 * @return The current CLINT time as a clint_mtime_t (uint64_t).
 */
static clint_mtime_t clint64_get_mtime(void) {
    return (((clint_mtime_t)*reg32(&__base_clint, CLINT_MTIME_HIGH_REG_OFFSET)) << 32) |
           ((clint_mtime_t)*reg32(&__base_clint, CLINT_MTIME_LOW_REG_OFFSET));
}

/**
 * @brief Spins (busy-waits) until the specified CLINT `mtime` value is reached.
 *
 * @param tgt_mtime Target CLINT time value.
 */
static void clint64_spin_until(clint_mtime_t tgt_mtime) {
    while (clint64_get_mtime() < tgt_mtime)
        ;
}

/**
 * @brief Spins (busy-waits) for a given number of ticks.
 *
 * @param ticks Number of clock cycles to wait.
 */
static void clint64_spin_ticks(uint32_t ticks) {
    clint64_spin_until(clint64_get_mtime() + ticks);
}

/**
 * @brief Estimates the core frequency based on a reference measurement period.
 *
 * This function assumes a stable clock; ref_time_inv is the inverse of the measurement period.
 *
 * @param ref_freq Reference frequency in Hz.
 * @param ref_time_inv Inverse of the measurement period.
 * @return Estimated core frequency in Hz.
 */
static uint32_t clint64_get_core_freq(uint32_t ref_freq, uint32_t ref_time_inv) {
    uint64_t start_mcycle, end_mcycle;
    uint64_t num_ticks = ref_freq / ref_time_inv;
    uint64_t start_time, end_time = clint64_get_mtime();

    do {
        start_mcycle = get_mcycle();
        start_time = clint64_get_mtime();
    } while (start_time == end_time);

    do {
        end_mcycle = get_mcycle();
        end_time = clint64_get_mtime();
    } while (end_time < start_time + num_ticks);

    return ((end_mcycle - start_mcycle) * ref_freq) / (end_time - start_time);
}

/**
 * @brief Sets the CLINT `mtimecmp` register for a specific timer index.
 *
 * The high register is written first, then the low register.
 *
 * @param timer_idx Timer index to configure.
 * @param value Target `mtimecmp` value.
 */
static void clint64_set_mtimecmpx(uint32_t timer_idx, clint_mtime_t value) {
    uint32_t vlo = (uint32_t)(value);
    uint32_t vhi = (uint32_t)(value >> 32);
    uint64_t offs = timer_idx << 3;
    *reg32(&__base_clint, CLINT_MTIMECMP_HIGH0_REG_OFFSET + offs) = vhi;
    *reg32(&__base_clint, CLINT_MTIMECMP_LOW0_REG_OFFSET + offs) = vlo;
}

/**
 * @brief Puts the core into sleep mode until the specified CLINT `mtime` value is reached.
 *
 * This function programs the mtimecmp register, then enables timer and global interrupts.
 *
 * @param timer_idx Timer index.
 * @param tgt_mtime Target CLINT time value.
 */
static void clint64_sleep_until(uint32_t timer_idx, clint_mtime_t tgt_mtime) {
    if (clint64_get_mtime() < tgt_mtime) return;
    clint64_set_mtimecmpx(timer_idx, tgt_mtime);
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
static void clint64_sleep_ticks(uint32_t timer_idx, uint32_t ticks) {
    clint64_sleep_until(timer_idx, clint64_get_mtime() + ticks);
}

// VIVIANEP: Need to skip doxygen generation for these functions
// to avoid duplicated defintion errors in the generated documentation

/// @cond DOXYGEN_SHOULD_SKIP_THIS
extern int clint_get_mtime()
    __attribute__((alias("clint64_get_mtime"), used, visibility("default")));
extern int clint_mtime_less_than(clint_mtime_t a, clint_mtime_t b)
    __attribute__((alias("clint64_mtime_less_than"), used, visibility("default")));
extern void clint_set_mtimecmpx(uint32_t timer_idx, clint_mtime_t value)
    __attribute__((alias("clint64_set_mtimecmpx"), used, visibility("default")));
extern void clint_sleep_until(uint32_t timer_idx, clint_mtime_t tgt_mtime)
    __attribute__((alias("clint64_sleep_until"), used, visibility("default")));
extern void clint_sleep_ticks(uint32_t timer_idx, uint32_t ticks)
    __attribute__((alias("clint64_sleep_ticks"), used, visibility("default")));
extern uint32_t clint_get_core_freq(uint32_t ref_freq, uint32_t ref_time_inv)
    __attribute__((alias("clint64_get_core_freq"), used, visibility("default")));
// // @endcond

/** @} */ // End of drivers_clint64 group
/** @} */ // End of drivers group