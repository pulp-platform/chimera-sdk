// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#include "soc.h"

#include "sw/device/lib/base/bitfield.h"

#define REG_WRITE32(addr, value) *((volatile uint32_t *)(addr)) = (uint32_t)(value)
#define REG_READ32(addr) *((volatile uint32_t *)(addr))

static inline uint32_t padframe_hyperbus_drv_strength_addr(void) {
    return PADFRAME_HYPERBUS_BASE + CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_REG_OFFSET;
}

/* CS0 */
void padframe_hyperbus_cs0_cfg_drv_set(uint8_t value) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_CS0_FIELD,
                                 value);
    REG_WRITE32(address, reg);
}

uint8_t padframe_hyperbus_cs0_cfg_drv_get(void) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    return (uint8_t)bitfield_field32_read(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_CS0_FIELD);
}

/* CS1 */
void padframe_hyperbus_cs1_cfg_drv_set(uint8_t value) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_CS1_FIELD,
                                 value);
    REG_WRITE32(address, reg);
}

uint8_t padframe_hyperbus_cs1_cfg_drv_get(void) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    return (uint8_t)bitfield_field32_read(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_CS1_FIELD);
}

/* CLK */
void padframe_hyperbus_clk_cfg_drv_set(uint8_t value) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_CLK_FIELD,
                                 value);
    REG_WRITE32(address, reg);
}

uint8_t padframe_hyperbus_clk_cfg_drv_get(void) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    return (uint8_t)bitfield_field32_read(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_CLK_FIELD);
}

/* CLKN */
void padframe_hyperbus_clkn_cfg_drv_set(uint8_t value) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_CLKN_FIELD,
                                 value);
    REG_WRITE32(address, reg);
}

uint8_t padframe_hyperbus_clkn_cfg_drv_get(void) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    return (uint8_t)bitfield_field32_read(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_CLKN_FIELD);
}

/* RST */
void padframe_hyperbus_rst_cfg_drv_set(uint8_t value) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_RST_FIELD,
                                 value);
    REG_WRITE32(address, reg);
}

uint8_t padframe_hyperbus_rst_cfg_drv_get(void) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    return (uint8_t)bitfield_field32_read(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_RST_FIELD);
}

/* RWDS */
void padframe_hyperbus_rwds_cfg_drv_set(uint8_t value) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_RWDS_FIELD,
                                 value);
    REG_WRITE32(address, reg);
}

uint8_t padframe_hyperbus_rwds_cfg_drv_get(void) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    return (uint8_t)bitfield_field32_read(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_RWDS_FIELD);
}

/* DQ0 */
void padframe_hyperbus_dq0_cfg_drv_set(uint8_t value) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ0_FIELD,
                                 value);
    REG_WRITE32(address, reg);
}

uint8_t padframe_hyperbus_dq0_cfg_drv_get(void) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    return (uint8_t)bitfield_field32_read(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ0_FIELD);
}

/* DQ1 */
void padframe_hyperbus_dq1_cfg_drv_set(uint8_t value) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ1_FIELD,
                                 value);
    REG_WRITE32(address, reg);
}

uint8_t padframe_hyperbus_dq1_cfg_drv_get(void) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    return (uint8_t)bitfield_field32_read(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ1_FIELD);
}

/* DQ2 */
void padframe_hyperbus_dq2_cfg_drv_set(uint8_t value) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ2_FIELD,
                                 value);
    REG_WRITE32(address, reg);
}

uint8_t padframe_hyperbus_dq2_cfg_drv_get(void) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    return (uint8_t)bitfield_field32_read(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ2_FIELD);
}

/* DQ3 */
void padframe_hyperbus_dq3_cfg_drv_set(uint8_t value) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ3_FIELD,
                                 value);
    REG_WRITE32(address, reg);
}

uint8_t padframe_hyperbus_dq3_cfg_drv_get(void) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    return (uint8_t)bitfield_field32_read(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ3_FIELD);
}

/* DQ4 */
void padframe_hyperbus_dq4_cfg_drv_set(uint8_t value) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ4_FIELD,
                                 value);
    REG_WRITE32(address, reg);
}

uint8_t padframe_hyperbus_dq4_cfg_drv_get(void) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    return (uint8_t)bitfield_field32_read(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ4_FIELD);
}

/* DQ5 */
void padframe_hyperbus_dq5_cfg_drv_set(uint8_t value) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ5_FIELD,
                                 value);
    REG_WRITE32(address, reg);
}

uint8_t padframe_hyperbus_dq5_cfg_drv_get(void) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    return (uint8_t)bitfield_field32_read(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ5_FIELD);
}

/* DQ6 */
void padframe_hyperbus_dq6_cfg_drv_set(uint8_t value) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ6_FIELD,
                                 value);
    REG_WRITE32(address, reg);
}

uint8_t padframe_hyperbus_dq6_cfg_drv_get(void) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    return (uint8_t)bitfield_field32_read(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ6_FIELD);
}

/* DQ7 */
void padframe_hyperbus_dq7_cfg_drv_set(uint8_t value) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ7_FIELD,
                                 value);
    REG_WRITE32(address, reg);
}

uint8_t padframe_hyperbus_dq7_cfg_drv_get(void) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    return (uint8_t)bitfield_field32_read(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_DQ7_FIELD);
}

/* RESERVED (bits [31:28]) */
void padframe_hyperbus_reserved_cfg_drv_set(uint8_t value) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_RESERVED_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t padframe_hyperbus_reserved_cfg_drv_get(void) {
    uint32_t address = padframe_hyperbus_drv_strength_addr();
    uint32_t reg = REG_READ32(address);
    return (uint8_t)bitfield_field32_read(
        reg, CHIMERAPD_HYPERBUS_PADS_DRV_STRENGTH_HYPERBUS_DRV_RESERVED_FIELD);
}
