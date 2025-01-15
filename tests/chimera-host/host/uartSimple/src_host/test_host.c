// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#include "uart.h"
#include <stdio.h>
#include <string.h>

// Simulated received data
const char simulated_data[] = "Hello, UART!";

bool read_callback(struct chi_device *device)
{
    uart_config_t *context = (uart_config_t *)device->device_addr;

    // Print received data
    // printf("Received data: %s\n", context->rx_buffer);

    return true;
}

int main()
{
    struct chi_device device;
    device.api = &uart_api;
    device.device_addr = (uint32_t *)0x40000000;
    device.cfg = &default_cfg;

    // Open the UART device
    int open_result = device.api->open(&device);
    // Check if UART was opened successfully
    if (open_result < 0)
    {
        return open_result;
    }

    // Allocate read buffer
    uint8_t rx_buffer[32] = {0};


    int close_result = device.api->close(&device);

    return open_result + close_result;
}