// SPDX-FileCopyrightText: 2022 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#pragma once

// Include Standard Libraries
#include <stdint.h>

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers

/**
 * @defgroup runtime_fll FLL Runtime
 * @ingroup runtime
 * @brief Host-side FLL parameter calculation and frequency switching utilities.
 *
 * These functions operate above the low-level FLL driver (`drivers_fll`) and
 * handle the higher-level tasks: computing multiplier/divider values, switching
 * both the SOC and cluster FLLs in concert, and reconfiguring the UART baud
 * rate after a frequency change.
 * @{
 */

/**
 * @brief Calculate FLL parameters for a target frequency.
 *
 * @param target_freq Target frequency in Hz
 * @param rtc_freq RTC frequency in Hz
 * @param mult Output: FLL multiplier
 * @param div Output: FLL divider (power of 2)
 * @return 0 on success, -1 on error
 */
int calculate_fll_params(uint32_t target_freq, uint32_t rtc_freq, uint32_t *mult, uint32_t *div);

/**
 * @brief Configure FLLs to target frequency and reconfigure UART.
 *
 * @param target_freq Target frequency in Hz
 * @param rtc_freq RTC frequency in Hz
 * @return Actual configured frequency in Hz, or 0 on error
 */
uint32_t configure_fll(uint32_t target_freq, uint32_t rtc_freq);

/**
 * @brief Restore the core frequency to its reset value and reconfigure UART.
 *
 * Measures the actual reset-domain clock frequency using the CLINT as a
 * reference, then reopens the UART with the measured frequency so that
 * the baud rate remains correct after an FLL is switched off.
 *
 * @param rtc_freq  Reference RTC frequency in Hz (used as timing reference).
 * @return          Measured core frequency in Hz, or 0 on error.
 */
uint32_t restore_default_freq(uint32_t rtc_freq);

/** @} */ // end defgroup runtime_fll
