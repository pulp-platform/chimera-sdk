// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/**
 * \defgroup drivers_hyperbus HyperBus Driver
 * @ingroup drivers_hyperbus
 * @ingroup drivers
 * @{
 * @brief HyperBus driver implementation for Chimera-SDK.
 *
 * This file provides the implementation of HyperBus initialization, read, and write
 * functions for the HyperBus peripheral.
 *
 */

// Include Standard Libraries
#include <stdint.h>
#include <stddef.h>

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "hyperbus.h"

// Include Runtime Headers
#include "util.h"

// Import HAL Headers

int hyperram_cfg_write(void *base, const hyperram_cfg_t *cfg) {
    if (base == NULL || cfg == NULL) {
        return -1;
    }

    // Base configuration registers
    printf("Writing HyperRAM configuration to hardware...\n");
    hyperbus_set_t_latency_access(base, cfg->t_latency_access);
    hyperbus_set_en_latency_additional(base, cfg->en_latency_additional);
    hyperbus_set_t_burst_max(base, cfg->t_burst_max);
    hyperbus_set_t_read_write_recovery(base, cfg->t_read_write_recovery);
    hyperbus_set_t_rx_clk_delay(base, cfg->t_rx_clk_delay);
    hyperbus_set_t_tx_clk_delay(base, cfg->t_tx_clk_delay);
    hyperbus_set_address_mask_msb(base, cfg->address_mask_msb);
    hyperbus_set_address_space(base, cfg->address_space);
    hyperbus_set_phys_in_use(base, cfg->phys_in_use);
    hyperbus_set_which_phy(base, cfg->which_phy);
    hyperbus_set_t_csh_cycles(base, cfg->t_csh_cycles);

    // Per-chip decode windows (optional)
    for (uint32_t i = 0; i < HYPERBUS_NUM_CHIPS; ++i) {
        uint32_t start = cfg->chip_base[i];
        uint32_t end = cfg->chip_base[i] + cfg->chip_space[i];
        hyperbus_set_chip_start(base, i, start);
        hyperbus_set_chip_end(base, i, end);
    }

    return 0;
}

hyperram_cfg_t hyperram_cfg_read(void *base) {
    hyperram_cfg_t cfg;

    // Read base configuration registers
    cfg.t_latency_access = *reg32(base, HYPERBUS_T_LATENCY_ACCESS_REG_OFFSET);
    cfg.en_latency_additional = *reg32(base, HYPERBUS_EN_LATENCY_ADDITIONAL_REG_OFFSET);
    cfg.t_burst_max = *reg32(base, HYPERBUS_T_BURST_MAX_REG_OFFSET);
    cfg.t_read_write_recovery = *reg32(base, HYPERBUS_T_READ_WRITE_RECOVERY_REG_OFFSET);
    cfg.t_rx_clk_delay = *reg32(base, HYPERBUS_T_RX_CLK_DELAY_REG_OFFSET);
    cfg.t_tx_clk_delay = *reg32(base, HYPERBUS_T_TX_CLK_DELAY_REG_OFFSET);
    cfg.address_mask_msb = *reg32(base, HYPERBUS_ADDRESS_MASK_MSB_REG_OFFSET);
    cfg.address_space = *reg32(base, HYPERBUS_ADDRESS_SPACE_REG_OFFSET);
    cfg.phys_in_use = *reg32(base, HYPERBUS_PHYS_IN_USE_REG_OFFSET);
    cfg.which_phy = *reg32(base, HYPERBUS_WHICH_PHY_REG_OFFSET);
    cfg.t_csh_cycles = *reg32(base, HYPERBUS_T_CSH_CYCLES_REG_OFFSET);

    for (uint32_t i = 0; i < HYPERBUS_NUM_CHIPS; ++i) {
        cfg.chip_base[i] = hyperbus_get_chip_start(base, i);
        cfg.chip_space[i] = hyperbus_get_chip_end(base, i) - cfg.chip_base[i];
    }

    return cfg;
}

void hyperram_cfg_print(void *base) {
    if (base == NULL) {
        return;
    }

    hyperram_cfg_t cfg = hyperram_cfg_read(base);

    printf_log("HyperRAM Configuration (%d chips):\n", HYPERBUS_NUM_CHIPS);
    printf_log("  t_latency_access:      0x%08X\n", cfg.t_latency_access);
    printf_log("  en_latency_additional: 0x%08X\n", cfg.en_latency_additional);
    printf_log("  t_burst_max:           0x%08X\n", cfg.t_burst_max);
    printf_log("  t_read_write_recovery: 0x%08X\n", cfg.t_read_write_recovery);
    printf_log("  t_rx_clk_delay:        0x%08X\n", cfg.t_rx_clk_delay);
    printf_log("  t_tx_clk_delay:        0x%08X\n", cfg.t_tx_clk_delay);
    printf_log("  address_mask_msb:      0x%08X\n", cfg.address_mask_msb);
    printf_log("  address_space:         0x%08X\n", cfg.address_space);
    printf_log("  phys_in_use:           0x%08X\n", cfg.phys_in_use);
    printf_log("  which_phy:             0x%08X\n", cfg.which_phy);
    printf_log("  t_csh_cycles:          0x%08X\n", cfg.t_csh_cycles);
    for (uint32_t i = 0; i < HYPERBUS_NUM_CHIPS; ++i) {
        printf_log("  chip_base(%d):          0x%08X\n", i, cfg.chip_base[i]);
        printf_log("  chip_space(%d):         0x%08X\n", i, cfg.chip_space[i]);
    }
}

void hyperram_cfg_default(hyperram_cfg_t *cfg, uint32_t chip_base, uint32_t chip_size) {
    // RTL reset values from hyperbus_cfg_regs.sv
    // phys_in_use/which_phy are 1-bit registers; choose 1 if num_phys > 1 else 0.

    cfg->t_latency_access = 0x6u;
    cfg->en_latency_additional = 0x0u;
    cfg->t_burst_max = 350u;
    cfg->t_read_write_recovery = 0x6u;
    cfg->t_rx_clk_delay = 0x8u;
    cfg->t_tx_clk_delay = 0x8u;
    cfg->address_mask_msb = 25u;
    cfg->address_space = 0x0u;
    cfg->phys_in_use = 0x0u;
    cfg->which_phy = 0x0u;
    cfg->t_csh_cycles = 0x1u;

    for (uint32_t i = 0; i < HYPERBUS_NUM_CHIPS; ++i) {
        cfg->chip_base[i] = chip_base + i * chip_size;
        cfg->chip_space[i] = chip_size;
    }
}

/** @} */ // End of drivers_hyperbus group
