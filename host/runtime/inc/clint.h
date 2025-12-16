// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#pragma once

#ifdef CHIMERA_DRIVER_CLINT

// Include Standard Libraries
#include <stdint.h>
#include <stdbool.h>

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers
#include "interrupt_api.h"

/**
 * \defgroup drivers_clint CLINT Driver
 * @ingroup runtime
 * @ingroup drivers
 * @brief Core Local Interruptor (CLINT) driver implementation for Chimera-SDK.
 * @{
 */

/**
 * @brief Platform-specific mtime type definition.
 *
 * - In a 64-bit system, clint_mtime_t is typedef'd to uint64_t.
 * - In a 32-bit system, clint_mtime_t is a structure with `.low` and `.high` members.
 */
#ifdef PLATFORM_IS_64BIT
/**
 * @brief 64-bit representation of mtime.
 */
typedef uint64_t clint_mtime_t;
#else
/**
 * @brief 32-bit representation of mtime (split into low/high).
 */
typedef struct {
    uint32_t low;  /**< Lower 32 bits of mtime. */
    uint32_t high; /**< Upper 32 bits of mtime. */
} clint_mtime_t;
#endif

/**
 * @brief CLINT’s implementation of the generic interrupt-controller API.
 *
 * Users should use the `chi_interrupt_t` + `interrupt_api` abstraction
 * when driving CLINT.
 */
extern const chi_interrupt_api_t default_clint_api;

extern chi_interrupt_t default_clint_inst;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Retrieves the current CLINT mtime value.
 *
 * @return The current CLINT timer value.
 */
clint_mtime_t clint_get_mtime(void);

/**
 * @brief Compares two CLINT mtime values.
 *
 * @param a First CLINT time value.
 * @param b Second CLINT time value.
 * @return 1 if a is less than b, 0 otherwise.
 */
int clint_mtime_less_than(clint_mtime_t a, clint_mtime_t b);

/**
 * @brief Spins (busy-waits) until the specified CLINT mtime value is reached.
 *
 * @param tgt_mtime Target CLINT time value.
 */
void clint_spin_until(clint_mtime_t tgt_mtime);

/**
 * @brief Spins (busy-waits) for a given number of ticks.
 *
 * @param ticks Number of clock cycles to wait.
 */
void clint_spin_ticks(uint32_t ticks);

/**
 * @brief Estimates the core frequency based on a reference measurement period.
 *
 * The parameter `ref_time_inv` is the inverse of the measurement period.
 *
 * @param ref_freq Reference frequency in Hz.
 * @param ref_time_inv Inverse of the measurement period.
 * @return Estimated core frequency in Hz.
 */
uint32_t clint_get_core_freq(uint32_t ref_freq, uint32_t ref_time_inv);

/**
 * @brief Sets the mtimecmp register for a specific timer index.
 *
 * @param timer_idx Timer index to configure.
 * @param value Target mtimecmp value.
 */
void clint_set_mtimecmpx(uint32_t timer_idx, clint_mtime_t value);

/**
 * @brief Puts the core to sleep until the specified CLINT mtime value is reached.
 *
 * @param timer_idx Timer index.
 * @param tgt_mtime Target CLINT time value for wakeup.
 */
void clint_sleep_until(uint32_t timer_idx, clint_mtime_t tgt_mtime);

/**
 * @brief Puts the core to sleep for a specified number of ticks.
 *
 * @param timer_idx Timer index.
 * @param ticks Number of clock cycles to sleep.
 */
void clint_sleep_ticks(uint32_t timer_idx, uint32_t ticks);

/**
 * @brief Default trap vector handler.
 *
 */
void default_trap_vector(void);

#ifdef __cplusplus
}
#endif

/** @} */ // end defgroup drivers_clint

#endif // CHIMERA_DRIVER_CLINT