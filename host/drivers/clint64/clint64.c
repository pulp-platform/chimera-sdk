// Copyright 2022 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Authors: Nicole Narr <narrn@student.ethz.ch>
//          Christopher Reinwardt <creinwar@student.ethz.ch>
//          Paul Scheffler <paulsc@iis.ee.ethz.ch>
//          Viviane Potocnik <vivianep@iis.ee.ethz.ch>

/**
 * \defgroup drivers_clint_64 64-bit CLINT Driver
 * @ingroup drivers
 * @brief 64-bit CLINT driver implementation for Chimera-SDK.
 * @{
 *
 */

// Include Standard Libraries
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "clint64.h"

// Include Runtime Headers
#include "util.h"
#include "clint.h"

// Import HAL Headers
#include "interrupt_api.h"

// VIVIANEP: Need to skip doxygen generation for these functions
// to avoid duplicated defintion errors in the generated documentation
/// @cond DOXYGEN_SHOULD_SKIP_THIS

/*---------------------------------------------------------------------------*/
/* 64‑bit CLINT core routines                                                 */
/*---------------------------------------------------------------------------*/

/**
 * @brief Retrieves the current CLINT `mtime` value.
 *
 * This function reads the high and low 64-bit registers and combines them into
 * a 64-bit value.
 *
 * @return The current CLINT time as a clint_mtime_t (uint64_t).
 */
clint_mtime_t clint_get_mtime(void) {
    return (((clint_mtime_t)*reg32(&__base_clint, CLINT_MTIME_HIGH_REG_OFFSET)) << 32) |
           ((clint_mtime_t)*reg32(&__base_clint, CLINT_MTIME_LOW_REG_OFFSET));
}

/**
 * @brief Compares two CLINT `mtime` values.
 *
 * @param a First CLINT time value.
 * @param b Second CLINT time value.
 * @return 1 if a is smaller than b, 0 otherwise.
 */
int clint_mtime_less_than(clint_mtime_t a, clint_mtime_t b) {
    return a < b;
}

/**
 * @brief Spins (busy-waits) until the specified CLINT `mtime` value is reached.
 *
 * @param tgt_mtime Target CLINT time value.
 */
void clint_spin_until(clint_mtime_t tgt_mtime) {
    while (clint_get_mtime() < tgt_mtime);
}

/**
 * @brief Spins (busy-waits) for a given number of ticks.
 *
 * @param ticks Number of clock cycles to wait.
 */
void clint_spin_ticks(uint32_t ticks) {
    clint_spin_until(clint_get_mtime() + ticks);
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
uint32_t clint_get_core_freq(uint32_t ref_freq, uint32_t ref_time_inv) {
    uint64_t start_mcycle, end_mcycle;
    uint64_t num_ticks = ref_freq / ref_time_inv;
    uint64_t start_time, end_time = clint_get_mtime();

    do {
        start_mcycle = get_mcycle();
        start_time = clint_get_mtime();
    } while (start_time == end_time);

    do {
        end_mcycle = get_mcycle();
        end_time = clint_get_mtime();
    } while (end_time < start_time + num_ticks);

    return ((end_mcycle - start_mcycle) * ref_freq) / (end_time - start_time);
}

/**
 * @brief Sets the CLINT `mtimecmp` register for a specific timer index.
 *
 * The high register is written first, then the low register.
 *
 * @param timer_idx Timer index to configure.
 * @param value     Target `mtimecmp` value.
 */
void clint_set_mtimecmpx(uint32_t timer_idx, clint_mtime_t value) {
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
void clint_sleep_until(uint32_t timer_idx, clint_mtime_t tgt_mtime) {
    if (clint_get_mtime() < tgt_mtime) return;
    clint_set_mtimecmpx(timer_idx, tgt_mtime);
    fence();
    set_mtie(1);
    set_mie(1);
    wfi();
}

/**
 * @brief Puts the core into sleep mode for a specified number of ticks.
 *
 * @param timer_idx Timer index.
 * @param ticks     Number of clock cycles to sleep.
 */
void clint_sleep_ticks(uint32_t timer_idx, uint32_t ticks) {
    clint_sleep_until(timer_idx, clint_get_mtime() + ticks);
}

/// @endcond

/*---------------------------------------------------------------------------*/
/* Provide driver‑specific chi_interrupt_api_t for CLINT                      */
/*---------------------------------------------------------------------------*/
static int clint64_init(const chi_interrupt_t *ctrl) {
    (void)ctrl;
    return 0;
}
static int clint64_register_handler(const chi_interrupt_t *ctrl, int irq, chi_irq_handler_t handler,
                                    void *arg) {
    (void)ctrl;
    (void)irq;
    (void)handler;
    (void)arg;
    return -1;
}
static int clint64_enable_irq(const chi_interrupt_t *ctrl, int irq) {
    (void)ctrl;
    (void)irq;
    return -1;
}
static int clint64_disable_irq(const chi_interrupt_t *ctrl, int irq) {
    (void)ctrl;
    (void)irq;
    return -1;
}
static int clint64_set_priority(const chi_interrupt_t *ctrl, int irq, int priority) {
    (void)ctrl;
    (void)irq;
    (void)priority;
    return -1;
}
static int clint64_acknowledge(const chi_interrupt_t *ctrl, int irq) {
    (void)ctrl;
    (void)irq;
    return -1;
}
static void clint64_dispatch(const chi_interrupt_t *ctrl) {
    (void)ctrl;
}

// VIVIANEP: Skip Doxygen generation for these alias functions to avoid duplicate definitions
/// @cond DOXYGEN_SHOULD_SKIP_THIS

/* Export the CLINT-specific interrupt API */
const chi_interrupt_api_t default_clint_api = {.init = clint64_init,
                                               .register_handler = clint64_register_handler,
                                               .enable_irq = clint64_enable_irq,
                                               .disable_irq = clint64_disable_irq,
                                               .set_priority = clint64_set_priority,
                                               .acknowledge = clint64_acknowledge,
                                               .dispatch = clint64_dispatch};
/// @endcond

/** @} */ // End of drivers_clint_64 group
