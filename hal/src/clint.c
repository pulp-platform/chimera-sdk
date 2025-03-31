// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#include "clint.h"

// Provide weak stubs that can be overridden by target-specific strong implementations

__attribute__((weak)) clint_mtime_t clint_get_mtime() {
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
