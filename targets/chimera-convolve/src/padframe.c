// Copyright 2025 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "padframe.h"

#define CHIMERA_PADFRAME_AON_CONFIG0_BASE_ADDR CHIMERA_PADFRAME_BASE_ADDRESS
#include "regs/padframe_aon_regs.h"

#include "sw/device/lib/base/bitfield.h"

#define REG_WRITE32(addr, value) *((volatile uint32_t *)addr) = (uint32_t)value;
#define REG_READ32(addr) *((volatile uint32_t *)addr)

void chimera_padframe_aon_gpio_0_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_0_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_0_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_0_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_0_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_0_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_0_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_0_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_0_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_0_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_0_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_0_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_0_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_0_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_0_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_0_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_0_mux_set(chimera_padframe_aon_gpio_0_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_0_mux_sel_t chimera_padframe_aon_gpio_0_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_0_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}

void chimera_padframe_aon_gpio_1_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_1_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_1_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_1_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_1_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_1_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_1_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_1_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_1_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_1_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_1_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_1_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_1_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_1_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_1_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_1_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_1_mux_set(chimera_padframe_aon_gpio_1_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_1_mux_sel_t chimera_padframe_aon_gpio_1_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_1_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}

void chimera_padframe_aon_gpio_2_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_2_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_2_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_2_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_2_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_2_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_2_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_2_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_2_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_2_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_2_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_2_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_2_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_2_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_2_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_2_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_2_mux_set(chimera_padframe_aon_gpio_2_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_2_mux_sel_t chimera_padframe_aon_gpio_2_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_2_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}

void chimera_padframe_aon_gpio_3_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_3_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_3_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_3_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_3_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_3_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_3_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_3_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_3_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_3_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_3_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_3_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_3_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_3_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_3_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_3_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_3_mux_set(chimera_padframe_aon_gpio_3_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_3_mux_sel_t chimera_padframe_aon_gpio_3_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_3_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}

void chimera_padframe_aon_gpio_4_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_4_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_4_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_4_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_4_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_4_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_4_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_4_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_4_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_4_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_4_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_4_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_4_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_4_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_4_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_4_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_4_mux_set(chimera_padframe_aon_gpio_4_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_4_mux_sel_t chimera_padframe_aon_gpio_4_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_4_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}

void chimera_padframe_aon_gpio_5_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_5_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_5_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_5_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_5_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_5_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_5_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_5_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_5_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_5_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_5_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_5_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_5_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_5_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_5_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_5_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_5_mux_set(chimera_padframe_aon_gpio_5_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_5_mux_sel_t chimera_padframe_aon_gpio_5_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_5_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}

void chimera_padframe_aon_gpio_6_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_6_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_6_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_6_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_6_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_6_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_6_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_6_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_6_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_6_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_6_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_6_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_6_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_6_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_6_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_6_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_6_mux_set(chimera_padframe_aon_gpio_6_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_6_mux_sel_t chimera_padframe_aon_gpio_6_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_6_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}

void chimera_padframe_aon_gpio_7_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_7_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_7_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_7_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_7_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_7_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_7_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_7_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_7_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_7_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_7_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_7_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_7_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_7_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_7_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_7_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_7_mux_set(chimera_padframe_aon_gpio_7_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_7_mux_sel_t chimera_padframe_aon_gpio_7_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_7_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}

void chimera_padframe_aon_gpio_8_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_8_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_8_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_8_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_8_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_8_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_8_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_8_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_8_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_8_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_8_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_8_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_8_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_8_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_8_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_8_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_8_mux_set(chimera_padframe_aon_gpio_8_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_8_mux_sel_t chimera_padframe_aon_gpio_8_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_8_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}

void chimera_padframe_aon_gpio_9_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_9_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_9_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_9_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_9_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_9_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_9_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_9_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_9_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_9_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_9_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_9_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_9_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_9_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_9_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_9_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_9_mux_set(chimera_padframe_aon_gpio_9_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_9_mux_sel_t chimera_padframe_aon_gpio_9_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_9_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}

void chimera_padframe_aon_gpio_10_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_10_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_10_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_10_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_10_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_10_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_10_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_10_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_10_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_10_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_10_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_10_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_10_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_10_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_10_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_10_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_10_mux_set(chimera_padframe_aon_gpio_10_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_10_mux_sel_t chimera_padframe_aon_gpio_10_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_10_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}

void chimera_padframe_aon_gpio_11_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_11_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_11_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_11_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_11_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_11_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_11_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_11_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_11_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_11_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_11_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_11_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_11_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_11_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_11_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_11_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_11_mux_set(chimera_padframe_aon_gpio_11_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_11_mux_sel_t chimera_padframe_aon_gpio_11_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_11_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}

void chimera_padframe_aon_gpio_12_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_12_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_12_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_12_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_12_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_12_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_12_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_12_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_12_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_12_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_12_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_12_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_12_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_12_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_12_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_12_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_12_mux_set(chimera_padframe_aon_gpio_12_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_12_mux_sel_t chimera_padframe_aon_gpio_12_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_12_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}

void chimera_padframe_aon_gpio_13_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_13_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_13_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_13_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_13_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_13_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_13_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_13_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_13_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_13_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_13_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_13_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_13_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_13_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_13_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_13_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_13_mux_set(chimera_padframe_aon_gpio_13_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_13_mux_sel_t chimera_padframe_aon_gpio_13_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_13_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}

void chimera_padframe_aon_gpio_14_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_14_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_14_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_14_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_14_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_14_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_14_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_14_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_14_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_14_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_14_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_14_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_14_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_14_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_14_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_14_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_14_mux_set(chimera_padframe_aon_gpio_14_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_14_mux_sel_t chimera_padframe_aon_gpio_14_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_14_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}

void chimera_padframe_aon_gpio_15_cfg_chip2pad_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_CHIP2PAD_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_15_cfg_chip2pad_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_CHIP2PAD_BIT);
}

void chimera_padframe_aon_gpio_15_cfg_drv_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_field32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_DRV_FIELD, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_15_cfg_drv_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_field32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_DRV_FIELD);
}

void chimera_padframe_aon_gpio_15_cfg_pde_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_PDE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_15_cfg_pde_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_PDE_BIT);
}

void chimera_padframe_aon_gpio_15_cfg_pue_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_PUE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_15_cfg_pue_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_PUE_BIT);
}

void chimera_padframe_aon_gpio_15_cfg_rxe_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_RXE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_15_cfg_rxe_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_RXE_BIT);
}

void chimera_padframe_aon_gpio_15_cfg_slw_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_SLW_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_15_cfg_slw_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_SLW_BIT);
}

void chimera_padframe_aon_gpio_15_cfg_smt_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_SMT_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_15_cfg_smt_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_SMT_BIT);
}

void chimera_padframe_aon_gpio_15_cfg_trie_set(uint8_t value) {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    reg = bitfield_bit32_write(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_TRIE_BIT, value);
    REG_WRITE32(address, reg);
}

uint8_t chimera_padframe_aon_gpio_15_cfg_trie_get() {
    uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_REG_OFFSET;
    uint32_t reg = REG_READ32(address);
    return bitfield_bit32_read(reg, CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_CFG_TRIE_BIT);
}

void chimera_padframe_aon_gpio_15_mux_set(chimera_padframe_aon_gpio_15_mux_sel_t mux_sel) {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_MUX_SEL_REG_OFFSET;
    REG_WRITE32(address, mux_sel & 15);
}

chimera_padframe_aon_gpio_15_mux_sel_t chimera_padframe_aon_gpio_15_mux_get() {
    const uint32_t address =
        CHIMERA_PADFRAME_BASE_ADDRESS + CHIMERA_PADFRAME_AON_CONFIG_GPIO_15_MUX_SEL_REG_OFFSET;

    return REG_READ32(address) & 15;
}
