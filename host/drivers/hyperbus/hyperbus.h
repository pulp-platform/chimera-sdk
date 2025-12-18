// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/**
 * \defgroup drivers_hyperbus HyperBus Driver
 * @ingroup drivers_hyperbus
 * @ingroup drivers
 * @brief HyperBus register definitions for Chimera-SDK.
 * @{
 *
 * This file defines register offsets and fields for an APB-based UART peripheral.
 * It is used in conjunction with the UART driver to configure and interact with
 * the hardware.
 */

// Include Standard Libraries
#include <stdint.h>

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers

// Include Runtime Headers
#include "util.h"

#ifndef HYPERBUS_H
#define HYPERBUS_H

#define HYPERBUS_NUM_PHYS 1u
#define HYPERBUS_NUM_CHIPS 2u

// -----------------------------------------------------------------------------
// HyperRAM / HyperBus configuration model
// -----------------------------------------------------------------------------
typedef struct hyperram_cfg {
    // Mirrors hyperbus_pkg::hyper_cfg_t fields (written as full 32-bit regs)
    uint32_t t_latency_access;      // RTL width [3:0]
    uint32_t en_latency_additional; // RTL width [0]
    uint32_t t_burst_max;           // RTL width [15:0]
    uint32_t t_read_write_recovery; // RTL width [3:0]
    uint32_t t_rx_clk_delay;        // RTL width [3:0]
    uint32_t t_tx_clk_delay;        // RTL width [3:0]
    uint32_t address_mask_msb;      // RTL width [4:0]
    uint32_t address_space;         // RTL width [0]
    uint32_t phys_in_use;           // RTL width [0]
    uint32_t which_phy;             // RTL width [0]
    uint32_t t_csh_cycles;          // RTL width [3:0]

    // Per-chip address window programming
    uint32_t chip_base[HYPERBUS_NUM_CHIPS];  // corresponds to RstChipBase usage
    uint32_t chip_space[HYPERBUS_NUM_CHIPS]; // corresponds to RstChipSpace usage (end noninclusive)
} hyperram_cfg_t;

static inline void hyperbus_set_t_latency_access(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_T_LATENCY_ACCESS_REG_OFFSET) = val;
}

static inline uint32_t hyperbus_get_t_latency_access(void *base) {
    return *reg32(base, HYPERBUS_T_LATENCY_ACCESS_REG_OFFSET);
}

static inline void hyperbus_set_en_latency_additional(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_EN_LATENCY_ADDITIONAL_REG_OFFSET) = val;
}

static inline uint32_t hyperbus_get_en_latency_additional(void *base) {
    return *reg32(base, HYPERBUS_EN_LATENCY_ADDITIONAL_REG_OFFSET);
}

static inline void hyperbus_set_t_burst_max(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_T_BURST_MAX_REG_OFFSET) = val;
}

static inline uint32_t hyperbus_get_t_burst_max(void *base) {
    return *reg32(base, HYPERBUS_T_BURST_MAX_REG_OFFSET);
}

static inline void hyperbus_set_t_read_write_recovery(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_T_READ_WRITE_RECOVERY_REG_OFFSET) = val;
}

static inline uint32_t hyperbus_get_t_read_write_recovery(void *base) {
    return *reg32(base, HYPERBUS_T_READ_WRITE_RECOVERY_REG_OFFSET);
}

static inline void hyperbus_set_t_rx_clk_delay(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_T_RX_CLK_DELAY_REG_OFFSET) = val;
}

static inline uint32_t hyperbus_get_t_rx_clk_delay(void *base) {
    return *reg32(base, HYPERBUS_T_RX_CLK_DELAY_REG_OFFSET);
}

static inline void hyperbus_set_t_tx_clk_delay(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_T_TX_CLK_DELAY_REG_OFFSET) = val;
}

static inline uint32_t hyperbus_get_t_tx_clk_delay(void *base) {
    return *reg32(base, HYPERBUS_T_TX_CLK_DELAY_REG_OFFSET);
}

static inline void hyperbus_set_address_mask_msb(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_ADDRESS_MASK_MSB_REG_OFFSET) = val;
}

static inline uint32_t hyperbus_get_address_mask_msb(void *base) {
    return *reg32(base, HYPERBUS_ADDRESS_MASK_MSB_REG_OFFSET);
}

static inline void hyperbus_set_address_space(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_ADDRESS_SPACE_REG_OFFSET) = val;
}

static inline uint32_t hyperbus_get_address_space(void *base) {
    return *reg32(base, HYPERBUS_ADDRESS_SPACE_REG_OFFSET);
}

static inline void hyperbus_set_phys_in_use(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_PHYS_IN_USE_REG_OFFSET) = val;
}

static inline uint32_t hyperbus_get_phys_in_use(void *base) {
    return *reg32(base, HYPERBUS_PHYS_IN_USE_REG_OFFSET);
}

static inline void hyperbus_set_which_phy(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_WHICH_PHY_REG_OFFSET) = val;
}

static inline uint32_t hyperbus_get_which_phy(void *base) {
    return *reg32(base, HYPERBUS_WHICH_PHY_REG_OFFSET);
}

static inline void hyperbus_set_t_csh_cycles(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_T_CSH_CYCLES_REG_OFFSET) = val;
}

static inline uint32_t hyperbus_get_t_csh_cycles(void *base) {
    return *reg32(base, HYPERBUS_T_CSH_CYCLES_REG_OFFSET);
}

// Per-chip range registers

static inline void hyperbus_set_chip_start(void *base, uint32_t chip, uint32_t start_addr) {
    *reg32(base, (int)HYPERBUS_CHIP_START_REG_OFFSET(chip)) = start_addr;
}

static inline uint32_t hyperbus_get_chip_start(void *base, uint32_t chip) {
    return *reg32(base, (int)HYPERBUS_CHIP_START_REG_OFFSET(chip));
}

static inline void hyperbus_set_chip_end(void *base, uint32_t chip,
                                         uint32_t end_addr_noninclusive) {
    *reg32(base, (int)HYPERBUS_CHIP_END_REG_OFFSET(chip)) = end_addr_noninclusive;
}

static inline uint32_t hyperbus_get_chip_end(void *base, uint32_t chip) {
    return *reg32(base, (int)HYPERBUS_CHIP_END_REG_OFFSET(chip));
}

static inline void hyperbus_set_chip_range(void *base, uint32_t chip, uint32_t start_addr,
                                           uint32_t end_addr_noninclusive) {
    hyperbus_set_chip_start(base, chip, start_addr);
    hyperbus_set_chip_end(base, chip, end_addr_noninclusive);
}

void hyperram_cfg_default(hyperram_cfg_t *cfg, uint32_t chip_base, uint32_t chip_size);
hyperram_cfg_t hyperram_cfg_read(void *base);
int hyperram_cfg_write(void *base, const hyperram_cfg_t *cfg);
void hyperram_cfg_print(void *base);

#endif // HYPERBUS_H

/** @} */ // End of drivers_hyperbus group
