// Copyright 2022 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Nicole Narr <narrn@student.ethz.ch>
// Christopher Reinwardt <creinwar@student.ethz.ch>
// Paul Scheffler <paulsc@iis.ee.ethz.ch>

#pragma once

#include <stdint.h>

typedef struct {
    uint32_t low;
    uint32_t high;
} clint_mtime_t;

clint_mtime_t clint_get_mtime();

int clint_mtime_less_than(clint_mtime_t a, clint_mtime_t b);

void clint_spin_until(clint_mtime_t tgt_mtime);

void clint_spin_ticks(uint32_t ticks);

// This assumes a stable clock; ref_time_inv is the measurement period's *inverse*
uint32_t clint_get_core_freq(uint32_t ref_freq, uint32_t ref_time_inv);

void clint_set_mtimecmpx(uint32_t timer_idx, clint_mtime_t value);

// PRE: requires an appropriate trap handler catching the timer interrupt
void clint_sleep_until(uint32_t timer_idx, clint_mtime_t tgt_mtime);

// PRE: requires an appropriate trap handler catching the timer interrupt
void clint_sleep_ticks(uint32_t timer_idx, uint32_t ticks);