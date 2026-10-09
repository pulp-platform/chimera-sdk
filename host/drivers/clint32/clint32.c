// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/*
 * 32-bit CLINT driver implementation.
 *
 * The 64-bit mtime counter is split into two 32-bit registers (MTIME_HIGH / MTIME_LOW).
 * clint_get_mtime() uses a double-read: read high, then low, then high again — if the
 * high word changed between the two reads the values straddle a carry and the read is
 * retried.  This makes the combined 64-bit read atomic without needing a critical section.
 *
 * clint32_init and related interrupt-API stubs are no-ops because the CLINT does not
 * support per-IRQ enable/disable at the controller level; all control goes through
 * mie/mip CSRs in the CPU core.
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

/*---------------------------------------------------------------------------*/
/* 32‑bit CLINT core routines                                                */
/*---------------------------------------------------------------------------*/

/* Reads the 64-bit mtime counter atomically by retrying if the high word changes between reads. */
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

/* Returns 1 if a is less than b, 0 otherwise. */
int clint_mtime_less_than(clint_mtime_t a, clint_mtime_t b) {
    return (a.high < b.high) || (a.high == b.high && a.low < b.low);
}

/* Busy-waits until mtime reaches tgt_mtime. */
void clint_spin_until(clint_mtime_t tgt_mtime) {
    while (clint_mtime_less_than(clint_get_mtime(), tgt_mtime)) {
        ; // Busy wait
    }
}

/* Busy-waits for the given number of mtime ticks. */
void clint_spin_ticks(uint32_t ticks) {
    clint_mtime_t start = clint_get_mtime();
    clint_mtime_t target = {start.low + ticks, start.high};
    if (target.low < start.low) {
        target.high++;
    }
    clint_spin_until(target);
}

/*
 * Estimates core frequency by counting mcycle ticks over a reference mtime interval.
 * The relative error is approximately ref_time_inv / ref_freq; choose ref_time_inv as
 * ref_freq * desired_relative_error to control accuracy.
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

    uint64_t duration = ((uint64_t)end_mcycle - (uint64_t)start_mcycle) * ref_freq;
    uint64_t ticks = ((uint64_t)end.low - (uint64_t)start.low);
    return (uint32_t)(duration / ticks);
}

/* Writes the mtimecmp register pair for the given timer index. */
void clint_set_mtimecmpx(uint32_t timer_idx, clint_mtime_t value) {
    uint32_t offs = timer_idx << 3;
    *reg32(&__base_clint, CLINT_MTIMECMP_HIGH0_REG_OFFSET + offs) = value.high;
    *reg32(&__base_clint, CLINT_MTIMECMP_LOW0_REG_OFFSET + offs) = value.low;
}

/* Programs mtimecmp, enables the timer interrupt, and issues wfi; returns immediately if tgt_mtime
 * is already past. */
void clint_sleep_until(uint32_t timer_idx, clint_mtime_t tgt_mtime) {
    if (clint_mtime_less_than(tgt_mtime, clint_get_mtime())) return;
    clint_set_mtimecmpx(timer_idx, tgt_mtime);
    fence();
    set_mtie(1);
    set_mie(1);
    wfi();
}

/* Sleeps for the given number of mtime ticks using wfi. */
void clint_sleep_ticks(uint32_t timer_idx, uint32_t ticks) {
    clint_mtime_t start = clint_get_mtime();
    clint_mtime_t target = {start.low + ticks, start.high};
    if (target.low < start.low) {
        target.high++;
    }
    clint_sleep_until(timer_idx, target);
}

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

/* Export the CLINT-specific interrupt API */
const chi_interrupt_api_t default_clint_api = {.init = clint32_init,
                                               .register_handler = clint32_register_handler,
                                               .enable_irq = clint32_enable_irq,
                                               .disable_irq = clint32_disable_irq,
                                               .set_priority = clint32_set_priority,
                                               .acknowledge = clint32_acknowledge,
                                               .dispatch = clint32_dispatch};
