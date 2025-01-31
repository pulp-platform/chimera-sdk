// Copyright 2022 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Nicole Narr <narrn@student.ethz.ch>
// Christopher Reinwardt <creinwar@student.ethz.ch>
// Paul Scheffler <paulsc@iis.ee.ethz.ch>

#include "clint.h"
#include "regs/clint.h"
#include "util.h"
#include "params.h"

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

// Compare mtime safely (since we can't do uint64_t arithmetic directly)
int clint_mtime_less_than(clint_mtime_t a, clint_mtime_t b) {
    return (a.high < b.high) || (a.high == b.high && a.low < b.low);
}

void clint_spin_until(clint_mtime_t tgt_mtime) {
    while (clint_mtime_less_than(clint_get_mtime(), tgt_mtime))
        ;
}

void clint_spin_ticks(uint32_t ticks) {
    clint_mtime_t start_mtime = clint_get_mtime();
    clint_mtime_t tgt_mtime = {start_mtime.low + ticks, start_mtime.high};

    // Handle carry in case of 32-bit overflow
    if (tgt_mtime.low < start_mtime.low) tgt_mtime.high++;

    clint_spin_until(tgt_mtime);
}

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

void clint_set_mtimecmpx(uint32_t timer_idx, clint_mtime_t value) {
    uint32_t mtimecmp_offs = timer_idx << 3;

    // Write high register first
    *reg32(&__base_clint, CLINT_MTIMECMP_HIGH0_REG_OFFSET + mtimecmp_offs) = value.high;
    *reg32(&__base_clint, CLINT_MTIMECMP_LOW0_REG_OFFSET + mtimecmp_offs) = value.low;
}

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

void clint_sleep_ticks(uint32_t timer_idx, uint32_t ticks) {
    clint_mtime_t start_mtime = clint_get_mtime();
    clint_mtime_t tgt_mtime = {start_mtime.low + ticks, start_mtime.high};

    // Handle carry
    if (tgt_mtime.low < start_mtime.low) tgt_mtime.high++;

    clint_sleep_until(timer_idx, tgt_mtime);
}