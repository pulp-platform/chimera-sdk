// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

/**
 * \defgroup hal_interrupt HAL Interrupt Controller
 * @ingroup hal
 * @brief Generic interrupt-controller interface (CLINT, CLIC, PLIC, …) in Chimera-SDK.
 * @{
 */

#ifndef INTERRUPT_API_H
#define INTERRUPT_API_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief IRQ handler function type.
 *
 * Handlers registered via `register_handler` will be invoked with
 * the IRQ number and a user-provided context pointer.
 *
 * @param irq Interrupt number.
 * @param arg User-provided context pointer.
 */
typedef void (*chi_irq_handler_t)(int irq, void *arg);

/** @brief Forward declaration of the interrupt controller instance. */
typedef struct chi_interrupt chi_interrupt_t;
/** @brief Forward declaration of the interrupt API structure. */
typedef struct chi_interrupt_api chi_interrupt_api_t;

/**
 * @ingroup hal_interrupt
 * @struct chi_interrupt
 * @brief Represents a generic interrupt controller.
 *
 * Holds the API function pointers, the optional MMIO base address,
 * and any controller-specific configuration.
 */
struct chi_interrupt {
    const chi_interrupt_api_t *api; /**< Controller operations. */
    uintptr_t base;                 /**< MMIO base address (if applicable). */
    void *cfg;                      /**< Controller-specific configuration. */
};

/**
 * @ingroup hal_interrupt
 * @struct chi_interrupt_api
 * @brief Defines function pointers for interrupt-controller operations.
 *
 * This API covers initialization, handler registration, enabling/disabling
 * IRQ lines, priority setting, acknowledgement, and optional dispatch.
 */
struct chi_interrupt_api {
    /**
     * @brief Initialize the interrupt controller.
     *
     * Perform any hardware setup needed before use.
     *
     * @param ctrl Controller instance.
     * @return 0 on success, negative on failure.
     */
    int (*init)(const chi_interrupt_t *ctrl);

    /**
     * @brief Register or replace an IRQ handler.
     *
     * Associates a handler function with a given interrupt number.
     *
     * @param ctrl Controller instance.
     * @param irq  Interrupt number.
     * @param h    Handler function.
     * @param arg  Context pointer for handler.
     * @return 0 on success, negative on failure.
     */
    int (*register_handler)(const chi_interrupt_t *ctrl, int irq, chi_irq_handler_t h, void *arg);

    /**
     * @brief Enable a specific IRQ line.
     *
     * @param ctrl Controller instance.
     * @param irq  Interrupt number.
     * @return 0 on success, negative on failure.
     */
    int (*enable_irq)(const chi_interrupt_t *ctrl, int irq);

    /**
     * @brief Disable a specific IRQ line.
     *
     * @param ctrl Controller instance.
     * @param irq  Interrupt number.
     * @return 0 on success, negative on failure.
     */
    int (*disable_irq)(const chi_interrupt_t *ctrl, int irq);

    /**
     * @brief Set the priority of an IRQ (if supported).
     *
     * Some controllers (PLIC, CLIC) support configurable priorities.
     *
     * @param ctrl Controller instance.
     * @param irq  Interrupt number.
     * @param prio Priority level.
     * @return 0 on success, -ENOSYS if not supported.
     */
    int (*set_priority)(const chi_interrupt_t *ctrl, int irq, int prio);

    /**
     * @brief Acknowledge (clear) a pending IRQ.
     *
     * Use for edge- or level-triggered interrupts.
     *
     * @param ctrl Controller instance.
     * @param irq  Interrupt number.
     * @return 0 on success, negative on failure.
     */
    int (*acknowledge)(const chi_interrupt_t *ctrl, int irq);

    /**
     * @brief Dispatch pending IRQs (optional).
     *
     * Some bare-metal setups drive interrupt handling in software via dispatch.
     *
     * @param ctrl Controller instance.
     */
    void (*dispatch)(const chi_interrupt_t *ctrl);
};

#endif // INTERRUPT_API_H

/** @} */ // end defgroup hal_interrupt
