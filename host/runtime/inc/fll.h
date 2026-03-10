// SPDX-FileCopyrightText: 2022 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#pragma once

// Include Standard Libraries
#include <stdint.h>

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers

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

uint32_t restore_default_freq(uint32_t rtc_freq);
