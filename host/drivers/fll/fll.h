// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

/**
 * \defgroup drivers_fll FLL Driver
 * @ingroup drivers
 * @brief FLL driver implementation for Chimera-SDK.
 * @{
 *
 */

uint32_t *getFllPtr(uint8_t fllIdx);
void initFll(volatile uint32_t *fllPtr);
void setFllFreq(volatile uint32_t *fllPtr, uint32_t mult, uint32_t div);

/** @} */
