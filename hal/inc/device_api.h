// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Moritz Scherer <scheremo@iis.ee.ethz.ch>

#include <stdint.h>
#include <stdio.h>

/**
 * \addtogroup device
 * @{
 */

/**
 * @brief Callback function for asynchronous device operations.
 *
 */
typedef bool (*chi_device_callback)(struct chi_device *device);

/**
 * @brief Device API structure.
 *
 * This structure defines the API for a device.
 */
typedef struct chi_device_api {
    /** Open the device.
     *
     * @param device Device to open.
     * @return int 0 on success, negative on failure.
     */
    int (*open)(struct chi_device *device);

    /** Close the device.
     *
     * @param device Device to close.
     * @return int 0 on success, negative on failure.
     */
    int (*close)(struct chi_device *device);

    /** Read from the device.
     *
     * @param device Device to read from.
     * @param buffer Buffer to read into.
     * @param size Size of the buffer.
     * @return ssize_t Number of bytes read, negative on failure.
     */
    ssize_t (*read_async)(struct chi_device *device, void *buffer, uint32_t size,
                          chi_device_callback cb);

    /** Write to the device.
     *
     * @param device Device to write to.
     * @param buffer Buffer to write from.
     * @param size Size of the buffer.
     * @return ssize_t Number of bytes written, negative on failure.
     */
    ssize_t (*write_async)(struct chi_device *device, const void *buffer, uint32_t size,
                           chi_device_callback cb);
} chi_device_api_t;

/**
 * @brief Device structure.
 *
 */
typedef struct chi_device {
    /**
     * @brief Device API.
     */
    struct chi_device_api *api; // function pointers
    /**
     * @brief Device address.
     */
    uint32_t *device_addr;
    /**
     * @brief Device configuration.
     */
    void *cfg;
} chi_device_t;

/** @} */