// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/**
 * \defgroup hal Hardware Abstraction Layer (HAL)
 * @brief The HAL module provides a unified API for Chimera‑SDK peripherals.
 * @{
 */

#ifndef DEVICE_API_H
#define DEVICE_API_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

/**
 * \defgroup hal_device Device API
 * @ingroup hal
 * @brief Defines the API for device interactions in Chimera‑SDK.
 * @{
 */

/**
 * @brief Forward declaration of chi_device_api_t.
 */
typedef struct chi_device_api chi_device_api_t;

/**
 * @brief Device structure.
 *
 * This structure represents a device in the system.
 */
typedef struct chi_device {
    chi_device_api_t *api; /**< Device API (function pointers). */
    uint32_t *device_addr; /**< Device address. */
    void *cfg;             /**< Device configuration. */
} chi_device_t;

/**
 * @brief Callback function type for asynchronous device operations.
 *
 * @param device Pointer to the device structure.
 * @return true if the operation was successful, false otherwise.
 */
typedef bool (*chi_device_callback_t)(chi_device_t *device);

/**
 * @ingroup hal_device
 * @struct chi_device_api
 * @brief Device API structure defining function pointers for a device.
 *
 * This structure contains function pointers for device operations,
 * enabling a standardized interface for device drivers.
 */
struct chi_device_api {
    /**
     * @brief Opens the device.
     *
     * This function initializes the device and prepares it for communication.
     *
     * @param device Pointer to the device structure.
     * @return 0 on success, negative value on failure.
     */
    int (*open)(chi_device_t *device);

    /**
     * @brief Closes the device.
     *
     * This function releases resources associated with the device.
     *
     * @param device Pointer to the device structure.
     * @return 0 on success, negative value on failure.
     */
    int (*close)(chi_device_t *device);

    /**
     * @brief Reads data from the device.
     *
     * This function performs an asynchronous read operation on the device.
     *
     * @param device Pointer to the device structure.
     * @param buffer Pointer to the buffer where the read data will be stored.
     * @param size Number of bytes to read.
     * @param cb Callback function to be called when the read completes.
     * @return Number of bytes read on success, negative value on failure.
     */
    ssize_t (*read)(chi_device_t *device, void *buffer, uint32_t size, chi_device_callback_t cb);

    /**
     * @brief Writes data to the device.
     *
     * This function performs an asynchronous write operation on the device.
     *
     * @param device Pointer to the device structure.
     * @param buffer Pointer to the buffer containing the data to write.
     * @param size Number of bytes to write.
     * @param cb Callback function to be called when the write completes.
     * @return Number of bytes written on success, negative value on failure.
     */
    ssize_t (*write)(chi_device_t *device, const void *buffer, uint32_t size,
                     chi_device_callback_t cb);
};

/** @} */ // end defgroup device

#endif // DEVICE_API_H

/** @} */ // end defgroup hal
