// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/**
 * \defgroup hal_interface HAL Communication Interface
 * @ingroup hal
 * @brief Communication interfaces (UART, SPI, …) in Chimera‑SDK.
 * @{
 */

#ifndef INTERFACE_API_H
#define INTERFACE_API_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

/** @brief Forward declaration of chi_interface_api_t. */
typedef struct chi_interface_api chi_interface_api_t;

/**
 * @brief Communication interface instance.
 *
 * This structure represents a generic communication interface
 * (e.g. UART, SPI) and holds the API pointer, the MMIO base,
 * and a driver‑specific configuration pointer.
 */
typedef struct chi_interface {
    const chi_interface_api_t *api; /**< Function pointers for this interface. */
    uintptr_t base;                 /**< MMIO base address (if applicable). */
    void *cfg;                      /**< Driver‑specific configuration. */
} chi_interface_t;

/**
 * @brief Callback function type for asynchronous interface operations.
 *
 * @param iface Pointer to the interface instance.
 * @return true if the operation was successful, false otherwise.
 */
typedef bool (*chi_interface_callback_t)(const chi_interface_t *iface);

/**
 * @ingroup hal_interface
 * @struct chi_interface_api
 * @brief API structure defining function pointers for communication interfaces.
 *
 * This structure contains the `open`, `close`, `read`, and `write`
 * function pointers, enabling a standardized interface for UART, SPI, etc.
 */
struct chi_interface_api {
    /**
     * @brief Opens the interface.
     *
     * This function initializes the hardware and prepares it for use.
     *
     * @param iface Pointer to the interface instance.
     * @return 0 on success, negative value on failure.
     */
    int (*open)(const chi_interface_t *iface);

    /**
     * @brief Closes the interface.
     *
     * This function releases any resources held by the interface.
     *
     * @param iface Pointer to the interface instance.
     * @return 0 on success, negative value on failure.
     */
    int (*close)(const chi_interface_t *iface);

    /**
     * @brief Reads data from the interface.
     *
     * Performs an asynchronous or blocking read, depending on the driver.
     *
     * @param iface  Pointer to the interface instance.
     * @param buffer Pointer to the buffer for incoming data.
     * @param size   Number of bytes to read.
     * @param cb     Callback when the operation completes.
     * @return Number of bytes read on success, negative on failure.
     */
    ssize_t (*read)(const chi_interface_t *iface, void *buffer, uint32_t size,
                    chi_interface_callback_t cb);

    /**
     * @brief Writes data to the interface.
     *
     * Performs an asynchronous or blocking write, depending on the driver.
     *
     * @param iface  Pointer to the interface instance.
     * @param buffer Pointer to the data to send.
     * @param size   Number of bytes to write.
     * @param cb     Callback when the operation completes.
     * @return Number of bytes written on success, negative on failure.
     */
    ssize_t (*write)(const chi_interface_t *iface, const void *buffer, uint32_t size,
                     chi_interface_callback_t cb);

    /**
     * @brief Flushes the interface buffers.
     *
     * @param iface Pointer to the interface instance.
     * @return 0 on success, negative value on failure.
     */
    int (*flush)(const chi_interface_t *iface);
};

/**
 * @brief Wrapper functions for interface operations.
 *
 * These functions call the corresponding function pointers
 * in the chi_interface_api structure.
 *
 * @param iface Pointer to the interface instance
 * @return int Return 0 on success, negative value on failure.
 */
int iface_open(const chi_interface_t *iface);

/**
 * @brief Wrapper functions for interface operations.
 *
 * These functions call the corresponding function pointers
 * in the chi_interface_api structure.
 *
 * @param iface Pointer to the interface instance.
 * @return int Return 0 on success, negative value on failure.
 */
int iface_close(const chi_interface_t *iface);

/**
 * @brief Wrapper functions for interface operations.
 *
 * These functions call the corresponding function pointers
 * in the chi_interface_api structure.
 *
 * @param iface  Pointer to the interface instance.
 * @param buffer Pointer to the buffer for incoming data.
 * @param size   Number of bytes to read.
 * @param cb     Callback when the operation completes.
 * @return ssize_t Number of bytes read on success, negative on failure.
 */
ssize_t iface_read(const chi_interface_t *iface, void *buffer, uint32_t size,
                   chi_interface_callback_t cb);

/**
 * @brief Wrapper functions for interface operations.
 *
 * These functions call the corresponding function pointers
 * in the chi_interface_api structure.
 *
 * @param iface  Pointer to the interface instance.
 * @param buffer Pointer to the data to send.
 * @param size   Number of bytes to write.
 * @param cb     Callback when the operation completes.
 * @return ssize_t Number of bytes written on success, negative on failure.
 */
ssize_t iface_write(const chi_interface_t *iface, const void *buffer, uint32_t size,
                    chi_interface_callback_t cb);

/**
 * @brief Wrapper function to flush the interface buffers.
 *
 * This function calls the corresponding function pointer
 * in the chi_interface_api structure.
 *
 * @param iface Pointer to the interface instance.
 * @return int Return 0 on success, negative value on failure.
 */
int iface_flush(const chi_interface_t *iface);

#endif // INTERFACE_API_H

/** @} */ // end defgroup hal_interface
