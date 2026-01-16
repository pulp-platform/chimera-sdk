// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Lorenzo Leone <lleone@iis.ee.ethz.ch>

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

/** @} */ // End of drivers_fll group
/** @} */ // End of drivers group
