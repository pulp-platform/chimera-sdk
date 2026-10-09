// SPDX-FileCopyrightText: 2022 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/*
 * 64-bit CLINT driver implementation.
 *
 * Reads mtime as a single uint64_t directly from a 64-bit-wide register.
 * Requires the platform to define PLATFORM_IS_64BIT so that clint.h selects
 * the uint64_t path for clint_mtime_t.
 *
 * The interrupt-API stubs (clint64_init etc.) are no-ops; CLINT interrupt
 * control is managed via the mie/mip CSRs in the CPU core rather than in
 * the controller itself.
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

/*---------------------------------------------------------------------------*/
/* 64‑bit CLINT core routines                                                 */
/*---------------------------------------------------------------------------*/

/* Reads mtime by combining the high and low 32-bit registers into a single uint64_t. */
clint_mtime_t clint_get_mtime(void) {
    return (((clint_mtime_t)*reg32(&__base_clint, CLINT_MTIME_HIGH_REG_OFFSET)) << 32) |
           ((clint_mtime_t)*reg32(&__base_clint, CLINT_MTIME_LOW_REG_OFFSET));
}

/* Returns 1 if a < b, 0 otherwise. */
int clint_mtime_less_than(clint_mtime_t a, clint_mtime_t b) {
    return a < b;
}

/* Busy-waits until mtime reaches tgt_mtime. */
void clint_spin_until(clint_mtime_t tgt_mtime) {
    while (clint_get_mtime() < tgt_mtime);
}

/* Busy-waits for the given number of mtime ticks. */
void clint_spin_ticks(uint32_t ticks) {
    clint_spin_until(clint_get_mtime() + ticks);
}

/*
 * Estimates core frequency by counting mcycle ticks over a reference mtime interval.
 * Assumes a stable clock; ref_time_inv is the inverse of the measurement period.
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

/* Writes the mtimecmp register for the given timer index; high word is written before low. */
void clint_set_mtimecmpx(uint32_t timer_idx, clint_mtime_t value) {
    uint32_t vlo = (uint32_t)(value);
    uint32_t vhi = (uint32_t)(value >> 32);
    uint64_t offs = timer_idx << 3;
    *reg32(&__base_clint, CLINT_MTIMECMP_HIGH0_REG_OFFSET + offs) = vhi;
    *reg32(&__base_clint, CLINT_MTIMECMP_LOW0_REG_OFFSET + offs) = vlo;
}

/* Programs mtimecmp, enables the timer interrupt, and issues wfi; returns immediately if tgt_mtime
 * is already past. */
void clint_sleep_until(uint32_t timer_idx, clint_mtime_t tgt_mtime) {
    if (clint_get_mtime() >= tgt_mtime) return;
    clint_set_mtimecmpx(timer_idx, tgt_mtime);
    fence();
    set_mtie(1);
    set_mie(1);
    wfi();
}

/* Sleeps for the given number of mtime ticks using wfi. */
void clint_sleep_ticks(uint32_t timer_idx, uint32_t ticks) {
    clint_sleep_until(timer_idx, clint_get_mtime() + ticks);
}

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

/* Export the CLINT-specific interrupt API */
const chi_interrupt_api_t default_clint_api = {.init = clint64_init,
                                               .register_handler = clint64_register_handler,
                                               .enable_irq = clint64_enable_irq,
                                               .disable_irq = clint64_disable_irq,
                                               .set_priority = clint64_set_priority,
                                               .acknowledge = clint64_acknowledge,
                                               .dispatch = clint64_dispatch};
