// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef CHIMERA_PADFRAME_HYPERBUS_H
#define CHIMERA_PADFRAME_HYPERBUS_H
#include <stdint.h>

/**
 * Sets the drv pad signal for the pad: hyperbus_cs0
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void padframe_hyperbus_cs0_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: hyperbus_cs0
 *
 * @return The value of the drv field
 */
uint8_t padframe_hyperbus_cs0_cfg_drv_get();

/**
 * Sets the drv pad signal for the pad: hyperbus_cs1
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void padframe_hyperbus_cs1_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: hyperbus_cs1
 *
 * @return The value of the drv field
 */
uint8_t padframe_hyperbus_cs1_cfg_drv_get();

/**
 * Sets the drv pad signal for the pad: hyperbus_clk
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void padframe_hyperbus_clk_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: hyperbus_clk
 *
 * @return The value of the drv field
 */
uint8_t padframe_hyperbus_clk_cfg_drv_get();

/**
 * Sets the drv pad signal for the pad: hyperbus_clkn
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void padframe_hyperbus_clkn_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: hyperbus_clkn
 *
 * @return The value of the drv field
 */
uint8_t padframe_hyperbus_clkn_cfg_drv_get();

/**
 * Sets the drv pad signal for the pad: hyperbus_rst
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void padframe_hyperbus_rst_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: hyperbus_rst
 *
 * @return The value of the drv field
 */
uint8_t padframe_hyperbus_rst_cfg_drv_get();

/**
 * Sets the drv pad signal for the pad: hyperbus_rwds
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void padframe_hyperbus_rwds_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: hyperbus_rwds
 *
 * @return The value of the drv field
 */
uint8_t padframe_hyperbus_rwds_cfg_drv_get();

/**
 * Sets the drv pad signal for the pad: hyperbus_dq0
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void padframe_hyperbus_dq0_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: hyperbus_dq0
 *
 * @return The value of the drv field
 */
uint8_t padframe_hyperbus_dq0_cfg_drv_get();

/**
 * Sets the drv pad signal for the pad: hyperbus_dq1
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void padframe_hyperbus_dq1_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: hyperbus_dq1
 *
 * @return The value of the drv field
 */
uint8_t padframe_hyperbus_dq1_cfg_drv_get();

/**
 * Sets the drv pad signal for the pad: hyperbus_dq2
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void padframe_hyperbus_dq2_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: hyperbus_dq2
 *
 * @return The value of the drv field
 */
uint8_t padframe_hyperbus_dq2_cfg_drv_get();

/**
 * Sets the drv pad signal for the pad: hyperbus_dq3
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void padframe_hyperbus_dq3_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: hyperbus_dq3
 *
 * @return The value of the drv field
 */
uint8_t padframe_hyperbus_dq3_cfg_drv_get();

/**
 * Sets the drv pad signal for the pad: hyperbus_dq4
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void padframe_hyperbus_dq4_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: hyperbus_dq4
 *
 * @return The value of the drv field
 */
uint8_t padframe_hyperbus_dq4_cfg_drv_get();

/**
 * Sets the drv pad signal for the pad: hyperbus_dq5
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void padframe_hyperbus_dq5_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: hyperbus_dq5
 *
 * @return The value of the drv field
 */
uint8_t padframe_hyperbus_dq5_cfg_drv_get();

/**
 * Sets the drv pad signal for the pad: hyperbus_dq6
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void padframe_hyperbus_dq6_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: hyperbus_dq6
 *
 * @return The value of the drv field
 */
uint8_t padframe_hyperbus_dq6_cfg_drv_get();

/**
 * Sets the drv pad signal for the pad: hyperbus_dq7
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void padframe_hyperbus_dq7_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: hyperbus_dq7
 *
 * @return The value of the drv field
 */
uint8_t padframe_hyperbus_dq7_cfg_drv_get();

/**
 * Sets the reserved pad signal field in the drive-strength register (bits [31:28]).
 *
 * @param value The value to program into the pad configuration register. A value smaller than 15.
 */
void padframe_hyperbus_reserved_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured reserved field value in the drive-strength register (bits [31:28]).
 *
 * @return The value of the reserved field
 */
uint8_t padframe_hyperbus_reserved_cfg_drv_get();

#endif /*  CHIMERA_PADFRAME_HYPERBUS_H */
