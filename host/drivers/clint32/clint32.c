// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0
//
// This file provides the strong (driver-specific) implementations for the
// CLINT functions using a 32-bit representation (a struct with 'low' and 'high').
// These functions will override the weak HAL symbols.

/**
 * \defgroup drivers_clint_32 32-bit CLINT Driver
 * @ingroup drivers
 * @brief 32-bit CLINT driver implementation for Chimera-SDK.
 * @{
 *
 * This file provides the implementation of the 32-bit CLINT driver.
 * It includes functions for reading the current time, comparing times,
 * spinning until a target time, and sleeping until a target time.
 *
 */

// Include Standard Libraries
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "clint32.h"

// Include Runtime Headers
#include "util.h"

// Import HAL Headers
#include "interrupt_api.h"

// VIVIANEP: Need to skip doxygen generation for these functions
// to avoid duplicated defintion errors in the generated documentation
/// @cond DOXYGEN_SHOULD_SKIP_THIS

/*---------------------------------------------------------------------------*/
/* 32‑bit CLINT core routines                                                */
/*---------------------------------------------------------------------------*/

/**
 * @brief Retrieves the current CLINT `mtime` value.
 *
 * This function safely reads the 64-bit timer value from two 32-bit registers,
 * ensuring atomicity by checking that the high register did not change between reads.
 *
 * @return The current CLINT time as a clint_mtime_t structure.
 */
clint_mtime_t clint_get_mtime(void) {
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
 * @return 1 if a is smaller than b, 0 otherwise.
 */
int clint_mtime_less_than(clint_mtime_t a, clint_mtime_t b) {
    return (a.high < b.high) || (a.high == b.high && a.low < b.low);
}

/**
 * @brief Spins (busy-waits) until the specified CLINT `mtime` value is reached.
 *
 * @param tgt_mtime Target CLINT time value.
 */
void clint_spin_until(clint_mtime_t tgt_mtime) {
    while (clint_mtime_less_than(clint_get_mtime(), tgt_mtime)) {
        ; // Busy wait
    }
}

/**
 * @brief Spins (busy-waits) for a given number of ticks.
 *
 * @param ticks Number of clock cycles to wait.
 */
void clint_spin_ticks(uint32_t ticks) {
    clint_mtime_t start = clint_get_mtime();
    clint_mtime_t target = {start.low + ticks, start.high};
    if (target.low < start.low) {
        target.high++;
    }
    clint_spin_until(target);
}

/**
 * @brief Estimates the core frequency based on a reference measurement period.
 *
 * @param ref_freq Reference frequency in Hz.
 * @param ref_time_inv Inverse of the measurement period.
 * @return Estimated core frequency in Hz.
 */
uint32_t clint_get_core_freq(uint32_t ref_freq, uint32_t ref_time_inv) {
    uint32_t start_mcycle, end_mcycle;
    uint32_t num_ticks = ref_freq / ref_time_inv;
    clint_mtime_t start, end = clint_get_mtime();

    do {
        start_mcycle = get_mcycle();
        start = clint_get_mtime();
    } while (start.low == end.low && start.high == end.high);

    do {
        end_mcycle = get_mcycle();
        end = clint_get_mtime();
    } while (clint_mtime_less_than(end, (clint_mtime_t){start.low + num_ticks, start.high}));

    return ((end_mcycle - start_mcycle) * ref_freq) / (end.low - start.low);
}

/**
 * @brief Sets the CLINT `mtimecmp` register for a specific timer index.
 *
 * @param timer_idx Timer index to configure.
 * @param value Target `mtimecmp` value.
 */
void clint_set_mtimecmpx(uint32_t timer_idx, clint_mtime_t value) {
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
void clint_sleep_until(uint32_t timer_idx, clint_mtime_t tgt_mtime) {
    if (clint_mtime_less_than(clint_get_mtime(), tgt_mtime)) return;
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
 * @param ticks Number of clock cycles to sleep.
 */
void clint_sleep_ticks(uint32_t timer_idx, uint32_t ticks) {
    clint_mtime_t start = clint_get_mtime();
    clint_mtime_t target = {start.low + ticks, start.high};
    if (target.low < start.low) {
        target.high++;
    }
    clint_sleep_until(timer_idx, target);
}

/// @endcond

/*---------------------------------------------------------------------------*/
/* Provide driver-specific chi_interrupt_api_t for CLINT                     */
/*---------------------------------------------------------------------------*/
static int clint32_init(const chi_interrupt_t *ctrl) {
    (void)ctrl;
    return 0;
}
static int clint32_register_handler(const chi_interrupt_t *ctrl, int irq, chi_irq_handler_t handler,
                                    void *arg) {
    (void)ctrl;
    (void)irq;
    (void)handler;
    (void)arg;
    return -1;
}
static int clint32_enable_irq(const chi_interrupt_t *ctrl, int irq) {
    (void)ctrl;
    (void)irq;
    return -1;
}
static int clint32_disable_irq(const chi_interrupt_t *ctrl, int irq) {
    (void)ctrl;
    (void)irq;
    return -1;
}
static int clint32_set_priority(const chi_interrupt_t *ctrl, int irq, int prio) {
    (void)ctrl;
    (void)irq;
    (void)prio;
    return -1;
}
static int clint32_acknowledge(const chi_interrupt_t *ctrl, int irq) {
    (void)ctrl;
    (void)irq;
    return -1;
}
static void clint32_dispatch(const chi_interrupt_t *ctrl) {
    (void)ctrl;
}

// VIVIANEP: Skip Doxygen generation for these alias functions to avoid duplicate definitions
/// @cond DOXYGEN_SHOULD_SKIP_THIS

/* Export the CLINT-specific interrupt API */
const chi_interrupt_api_t default_clint_api = {.init = clint32_init,
                                               .register_handler = clint32_register_handler,
                                               .enable_irq = clint32_enable_irq,
                                               .disable_irq = clint32_disable_irq,
                                               .set_priority = clint32_set_priority,
                                               .acknowledge = clint32_acknowledge,
                                               .dispatch = clint32_dispatch};
/// @endcond

/** @} */ // end drivers_clint_32 group
