// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potcnik <vivianep@iis.ee.ethz.ch>

#include "uart.h"

int main()
{
    struct chi_device device;
    device.api = &uart_api;
    device.device_addr = (uint32_t *)0x40000000;
    device.cfg = &default_cfg;

    int open_result = device.api->open(&device);
    // int close_result = device.api->close(&device);

    // int open_result = 0;
    int close_result = 0;

    return open_result + close_result;
}