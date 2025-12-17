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

int hyperram_configure(void *base, const hyperram_cfg_t *cfg) {
    if (base == NULL || cfg == NULL) {
        return -1;
    }

    // Base configuration registers
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
    for (uint32_t i = 0; i < cfg->num_chips; ++i) {
        uint32_t start = cfg->chip_base + (cfg->chip_space * i);
        uint32_t end = cfg->chip_base + (cfg->chip_space * (i + 1u));
        hyperbus_set_chip_start(base, i, start);
        hyperbus_set_chip_end(base, i, end);
    }

    return 0;
}

hyperram_cfg_t hyperram_cfg_default(uint32_t chip_base, uint32_t chip_size) {
    // RTL reset values from hyperbus_cfg_regs.sv
    // phys_in_use/which_phy are 1-bit registers; choose 1 if num_phys > 1 else 0.

    return (hyperram_cfg_t){
        .t_latency_access = 0x6u,
        .en_latency_additional = 0x0u,
        .t_burst_max = 350u,
        .t_read_write_recovery = 0x6u,
        .t_rx_clk_delay = 0x8u,
        .t_tx_clk_delay = 0x8u,
        .address_mask_msb = 25u,
        .address_space = 0x0u,
        .phys_in_use = 0x0u,
        .which_phy = 0x0u,
        .t_csh_cycles = 0x1u,
        .num_chips = 0x2,
        .chip_base = chip_base,
        .chip_space = chip_size,
    };
}

/** @} */ // End of drivers_hyperbus group
