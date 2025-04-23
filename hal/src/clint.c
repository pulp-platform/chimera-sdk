// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#include "clint.h"

/*---------------------------------------------------------------------------*/
/* CLINT‐specific weak stubs (can be overridden by platform implementations) */
/*---------------------------------------------------------------------------*/

__attribute__((weak)) clint_mtime_t clint_get_mtime(void) {
    clint_mtime_t dummy = {0};
    return dummy;
}

__attribute__((weak)) int clint_mtime_less_than(clint_mtime_t a, clint_mtime_t b) {
    (void)a;
    (void)b;
    return 0;
}

__attribute__((weak)) void clint_spin_until(clint_mtime_t tgt_mtime) {
    (void)tgt_mtime;
}

__attribute__((weak)) void clint_spin_ticks(uint32_t ticks) {
    (void)ticks;
}

__attribute__((weak)) uint32_t clint_get_core_freq(uint32_t ref_freq, uint32_t ref_time_inv) {
    (void)ref_freq;
    (void)ref_time_inv;
    return 0;
}

__attribute__((weak)) void clint_set_mtimecmpx(uint32_t timer_idx, clint_mtime_t value) {
    (void)timer_idx;
    (void)value;
}

__attribute__((weak)) void clint_sleep_until(uint32_t timer_idx, clint_mtime_t tgt_mtime) {
    (void)timer_idx;
    (void)tgt_mtime;
}

__attribute__((weak)) void clint_sleep_ticks(uint32_t timer_idx, uint32_t ticks) {
    (void)timer_idx;
    (void)ticks;
}

/*---------------------------------------------------------------------------*/
/* Generic interrupt‐controller stubs                                      */
/*---------------------------------------------------------------------------*/

__attribute__((weak)) int clint_init(chi_interrupt_t *ctrl) {
    (void)ctrl;
    return 0;
}

__attribute__((weak)) int clint_register_handler(chi_interrupt_t *ctrl, int irq,
                                                 chi_irq_handler_t handler, void *arg) {
    (void)ctrl;
    (void)irq;
    (void)handler;
    (void)arg;
    return -1;
}

__attribute__((weak)) int clint_enable_irq(chi_interrupt_t *ctrl, int irq) {
    (void)ctrl;
    (void)irq;
    return -1;
}

__attribute__((weak)) int clint_disable_irq(chi_interrupt_t *ctrl, int irq) {
    (void)ctrl;
    (void)irq;
    return -1;
}

__attribute__((weak)) int clint_set_priority(chi_interrupt_t *ctrl, int irq, int priority) {
    (void)ctrl;
    (void)irq;
    (void)priority;
    return -1;
}

__attribute__((weak)) int clint_acknowledge(chi_interrupt_t *ctrl, int irq) {
    (void)ctrl;
    (void)irq;
    return -1;
}

__attribute__((weak)) void clint_dispatch(chi_interrupt_t *ctrl) {
    (void)ctrl;
}

/*---------------------------------------------------------------------------*/
/* Export the generic interrupt‐controller API for CLINT                    */
/*---------------------------------------------------------------------------*/

__attribute__((weak)) chi_interrupt_api_t clint_api = {
    .init = clint_init,
    .register_handler = clint_register_handler,
    .enable_irq = clint_enable_irq,
    .disable_irq = clint_disable_irq,
    .set_priority = clint_set_priority,
    .acknowledge = clint_acknowledge,
    .dispatch = clint_dispatch,
};
