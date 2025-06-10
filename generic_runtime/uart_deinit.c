// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Victor Jung <jungvi@iis.ee.ethz.ch>

#include "uart.h"

#include "util.h"
#include "params.h"
#include "regs/cheshire.h"

void _uart_deinit(void){
    uart_close(&uart_iface);
}