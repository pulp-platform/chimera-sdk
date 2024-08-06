// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Moritz Scherer <scheremo@iis.ee.ethz.ch>

#include <stdint.h>
#include <stdio.h>

typedef bool (*chi_device_callback)(struct chi_device *device);

typedef struct chi_device_api {
    int (*open)(struct chi_device *device);
    int (*close)(struct chi_device *device);
    ssize_t (*read_async)(struct chi_device *device, void *buffer, uint32_t size,
                          chi_device_callback cb);
    ssize_t (*write_async)(struct chi_device *device, const void *buffer, uint32_t size,
                           chi_device_callback cb);
} chi_device_api_t;

typedef struct chi_device {
    struct chi_device_api *api; // function pointers
    uint32_t *device_addr;
    void *cfg;
} chi_device_t;
