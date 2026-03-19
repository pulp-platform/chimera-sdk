// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/*
 * HAL interface wrapper implementation.
 *
 * Each iface_*() function validates the function pointer chain before
 * dispatching: if the chi_interface_t pointer, its api pointer, or the
 * specific function pointer is NULL the call returns -1 without dereferencing.
 * This guards against partially-initialised interface instances and makes
 * missing-driver failures predictable rather than undefined.
 */

#include "interface_api.h"

int iface_open(const chi_interface_t *iface) {
    if (!iface || !iface->api || !iface->api->open) {
        return -1;
    }
    return iface->api->open(iface);
}

int iface_close(const chi_interface_t *iface) {
    if (!iface || !iface->api || !iface->api->close) {
        return -1;
    }
    return iface->api->close(iface);
}

ssize_t iface_read(const chi_interface_t *iface, void *buffer, uint32_t size,
                   chi_interface_callback_t cb) {
    if (!iface || !iface->api || !iface->api->read) {
        return -1;
    }
    return iface->api->read(iface, buffer, size, cb);
}

ssize_t iface_write(const chi_interface_t *iface, const void *buffer, uint32_t size,
                    chi_interface_callback_t cb) {
    if (!iface || !iface->api || !iface->api->write) {
        return -1;
    }
    return iface->api->write(iface, buffer, size, cb);
}

int iface_flush(const chi_interface_t *iface) {
    if (!iface || !iface->api || !iface->api->flush) {
        return -1;
    }
    return iface->api->flush(iface);
}
