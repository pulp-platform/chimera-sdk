// Copyright 2025 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Victor Jung <jungvi@iis.ee.ethz.ch>

#include "uart.h"
#include "uart_setup.h"

#include "util.h"
#include "params.h"
#include "regs/cheshire.h"

void _uart_init(void){
    static uart_config_t uart_cfg;
    uart_cfg = default_cfg;
    
    uint32_t rtc_freq = *reg32(&__base_regs, CHESHIRE_RTC_FREQ_REG_OFFSET);
    uint32_t reset_freq = clint_get_core_freq(rtc_freq, 2500);
    uart_cfg.clk_freq_hz = reset_freq;
    
    uart_iface.base = (uintptr_t)&__base_uart;
    uart_iface.cfg = &uart_cfg;
    uart_iface.api = &uart_api;

    uart_open(&uart_iface);
}