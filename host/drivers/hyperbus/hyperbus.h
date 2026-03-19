// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/**
 * \defgroup drivers_hyperbus HyperBus Driver
 * @ingroup drivers
 * @brief HyperBus register definitions for Chimera-SDK.
 * @{
 *
 * This file defines register offsets and fields for an APB-based HyperBus peripheral.
 * It is used in conjunction with the HyperBus driver to configure and interact with
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

/**
 * @brief Set the latency access time for HyperBus transactions.
 *
 * @param[in] base Base address of the HyperBus peripheral
 * @param[in] val  Latency access time value (4-bit field)
 */
static inline void hyperbus_set_t_latency_access(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_T_LATENCY_ACCESS_REG_OFFSET) = val;
}

/**
 * @brief Get the latency access time for HyperBus transactions.
 *
 * @param[in]  base Base address of the HyperBus peripheral
 * @return     Latency access time value (4-bit field)
 */
static inline uint32_t hyperbus_get_t_latency_access(void *base) {
    return *reg32(base, HYPERBUS_T_LATENCY_ACCESS_REG_OFFSET);
}

/**
 * @brief Enable or disable additional latency compensation.
 *
 * @param[in] base Base address of the HyperBus peripheral
 * @param[in] val  Enable flag (1-bit field, 0 = disabled, 1 = enabled)
 */
static inline void hyperbus_set_en_latency_additional(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_EN_LATENCY_ADDITIONAL_REG_OFFSET) = val;
}

/**
 * @brief Get the additional latency compensation enable status.
 *
 * @param[in]  base Base address of the HyperBus peripheral
 * @return     Enable status (1-bit field, 0 = disabled, 1 = enabled)
 */
static inline uint32_t hyperbus_get_en_latency_additional(void *base) {
    return *reg32(base, HYPERBUS_EN_LATENCY_ADDITIONAL_REG_OFFSET);
}

/**
 * @brief Set the maximum burst length for HyperBus transfers.
 *
 * @param[in] base Base address of the HyperBus peripheral
 * @param[in] val  Maximum burst length value (16-bit field)
 */
static inline void hyperbus_set_t_burst_max(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_T_BURST_MAX_REG_OFFSET) = val;
}

/**
 * @brief Get the maximum burst length for HyperBus transfers.
 *
 * @param[in]  base Base address of the HyperBus peripheral
 * @return     Maximum burst length value (16-bit field)
 */
static inline uint32_t hyperbus_get_t_burst_max(void *base) {
    return *reg32(base, HYPERBUS_T_BURST_MAX_REG_OFFSET);
}

/**
 * @brief Set the read/write recovery time.
 *
 * @param[in] base Base address of the HyperBus peripheral
 * @param[in] val  Read/write recovery time value (4-bit field)
 */
static inline void hyperbus_set_t_read_write_recovery(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_T_READ_WRITE_RECOVERY_REG_OFFSET) = val;
}

/**
 * @brief Get the read/write recovery time.
 *
 * @param[in]  base Base address of the HyperBus peripheral
 * @return     Read/write recovery time value (4-bit field)
 */
static inline uint32_t hyperbus_get_t_read_write_recovery(void *base) {
    return *reg32(base, HYPERBUS_T_READ_WRITE_RECOVERY_REG_OFFSET);
}

/**
 * @brief Set the receive clock delay for HyperBus data.
 *
 * @param[in] base Base address of the HyperBus peripheral
 * @param[in] val  RX clock delay value (4-bit field)
 */
static inline void hyperbus_set_t_rx_clk_delay(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_T_RX_CLK_DELAY_REG_OFFSET) = val;
}

/**
 * @brief Get the receive clock delay for HyperBus data.
 *
 * @param[in]  base Base address of the HyperBus peripheral
 * @return     RX clock delay value (4-bit field)
 */
static inline uint32_t hyperbus_get_t_rx_clk_delay(void *base) {
    return *reg32(base, HYPERBUS_T_RX_CLK_DELAY_REG_OFFSET);
}

/**
 * @brief Set the transmit clock delay for HyperBus data.
 *
 * @param[in] base Base address of the HyperBus peripheral
 * @param[in] val  TX clock delay value (4-bit field)
 */
static inline void hyperbus_set_t_tx_clk_delay(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_T_TX_CLK_DELAY_REG_OFFSET) = val;
}

/**
 * @brief Get the transmit clock delay for HyperBus data.
 *
 * @param[in]  base Base address of the HyperBus peripheral
 * @return     TX clock delay value (4-bit field)
 */
static inline uint32_t hyperbus_get_t_tx_clk_delay(void *base) {
    return *reg32(base, HYPERBUS_T_TX_CLK_DELAY_REG_OFFSET);
}

/**
 * @brief Set the address mask MSB for address decoding.
 *
 * @param[in] base Base address of the HyperBus peripheral
 * @param[in] val  Address mask MSB value (5-bit field)
 */
static inline void hyperbus_set_address_mask_msb(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_ADDRESS_MASK_MSB_REG_OFFSET) = val;
}

/**
 * @brief Get the address mask MSB for address decoding.
 *
 * @param[in]  base Base address of the HyperBus peripheral
 * @return     Address mask MSB value (5-bit field)
 */
static inline uint32_t hyperbus_get_address_mask_msb(void *base) {
    return *reg32(base, HYPERBUS_ADDRESS_MASK_MSB_REG_OFFSET);
}

/**
 * @brief Set the address space configuration.
 *
 * @param[in] base Base address of the HyperBus peripheral
 * @param[in] val  Address space value (1-bit field)
 */
static inline void hyperbus_set_address_space(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_ADDRESS_SPACE_REG_OFFSET) = val;
}

/**
 * @brief Get the address space configuration.
 *
 * @param[in]  base Base address of the HyperBus peripheral
 * @return     Address space value (1-bit field)
 */
static inline uint32_t hyperbus_get_address_space(void *base) {
    return *reg32(base, HYPERBUS_ADDRESS_SPACE_REG_OFFSET);
}

/**
 * @brief Set which physical interfaces are in use.
 *
 * @param[in] base Base address of the HyperBus peripheral
 * @param[in] val  Physical interfaces in use bitmask (1-bit field)
 */
static inline void hyperbus_set_phys_in_use(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_PHYS_IN_USE_REG_OFFSET) = val;
}

/**
 * @brief Get which physical interfaces are in use.
 *
 * @param[in]  base Base address of the HyperBus peripheral
 * @return     Physical interfaces in use bitmask (1-bit field)
 */
static inline uint32_t hyperbus_get_phys_in_use(void *base) {
    return *reg32(base, HYPERBUS_PHYS_IN_USE_REG_OFFSET);
}

/**
 * @brief Set the active physical interface.
 *
 * @param[in] base Base address of the HyperBus peripheral
 * @param[in] val  Active PHY index (1-bit field)
 */
static inline void hyperbus_set_which_phy(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_WHICH_PHY_REG_OFFSET) = val;
}

/**
 * @brief Get the active physical interface.
 *
 * @param[in]  base Base address of the HyperBus peripheral
 * @return     Active PHY index (1-bit field)
 */
static inline uint32_t hyperbus_get_which_phy(void *base) {
    return *reg32(base, HYPERBUS_WHICH_PHY_REG_OFFSET);
}

/**
 * @brief Set the chip select hold time in cycles.
 *
 * @param[in] base Base address of the HyperBus peripheral
 * @param[in] val  Chip select hold time (4-bit field)
 */
static inline void hyperbus_set_t_csh_cycles(void *base, uint32_t val) {
    *reg32(base, HYPERBUS_T_CSH_CYCLES_REG_OFFSET) = val;
}

/**
 * @brief Get the chip select hold time in cycles.
 *
 * @param[in]  base Base address of the HyperBus peripheral
 * @return     Chip select hold time (4-bit field)
 */
static inline uint32_t hyperbus_get_t_csh_cycles(void *base) {
    return *reg32(base, HYPERBUS_T_CSH_CYCLES_REG_OFFSET);
}

// Per-chip range registers

/**
 * @brief Set the start address for a chip's address window.
 *
 * @param[in] base       Base address of the HyperBus peripheral
 * @param[in] chip       Chip index (0 to HYPERBUS_NUM_CHIPS - 1)
 * @param[in] start_addr Start address of the chip's memory window
 */
static inline void hyperbus_set_chip_start(void *base, uint32_t chip, uint32_t start_addr) {
    *reg32(base, (int)HYPERBUS_CHIP_START_REG_OFFSET(chip)) = start_addr;
}

/**
 * @brief Get the start address for a chip's address window.
 *
 * @param[in]  base Chip index (0 to HYPERBUS_NUM_CHIPS - 1)
 * @param[in]  chip Chip index
 * @return     Start address of the chip's memory window
 */
static inline uint32_t hyperbus_get_chip_start(void *base, uint32_t chip) {
    return *reg32(base, (int)HYPERBUS_CHIP_START_REG_OFFSET(chip));
}

/**
 * @brief Set the end address for a chip's address window.
 *
 * @param[in] base                   Base address of the HyperBus peripheral
 * @param[in] chip                   Chip index (0 to HYPERBUS_NUM_CHIPS - 1)
 * @param[in] end_addr_noninclusive  End address (non-inclusive) of the chip's memory window
 */
static inline void hyperbus_set_chip_end(void *base, uint32_t chip,
                                         uint32_t end_addr_noninclusive) {
    *reg32(base, (int)HYPERBUS_CHIP_END_REG_OFFSET(chip)) = end_addr_noninclusive;
}

/**
 * @brief Get the end address for a chip's address window.
 *
 * @param[in]  base Base address of the HyperBus peripheral
 * @param[in]  chip Chip index (0 to HYPERBUS_NUM_CHIPS - 1)
 * @return     End address (non-inclusive) of the chip's memory window
 */
static inline uint32_t hyperbus_get_chip_end(void *base, uint32_t chip) {
    return *reg32(base, (int)HYPERBUS_CHIP_END_REG_OFFSET(chip));
}

/**
 * @brief Set both start and end addresses for a chip's address window.
 *
 * @param[in] base                   Base address of the HyperBus peripheral
 * @param[in] chip                   Chip index (0 to HYPERBUS_NUM_CHIPS - 1)
 * @param[in] start_addr             Start address of the chip's memory window
 * @param[in] end_addr_noninclusive  End address (non-inclusive) of the chip's memory window
 */
static inline void hyperbus_set_chip_range(void *base, uint32_t chip, uint32_t start_addr,
                                           uint32_t end_addr_noninclusive) {
    hyperbus_set_chip_start(base, chip, start_addr);
    hyperbus_set_chip_end(base, chip, end_addr_noninclusive);
}

/**
 * @brief Initialize HyperRAM configuration with default values.
 *
 * Sets up sensible defaults for a HyperRAM device with the specified base address and size.
 *
 * @param[out] cfg       Pointer to the configuration structure to initialize
 * @param[in]  chip_base Physical base address of the chip
 * @param[in]  chip_size Size of the chip's address space in bytes
 */
void hyperram_cfg_default(hyperram_cfg_t *cfg, uint32_t chip_base, uint32_t chip_size);

/**
 * @brief Read the current HyperRAM configuration from hardware.
 *
 * Reads all configuration registers from the HyperBus peripheral and returns a populated
 * configuration structure.
 *
 * @param[in]  base Base address of the HyperBus peripheral
 * @return     Configuration structure containing the hardware settings
 */
hyperram_cfg_t hyperram_cfg_read(void *base);

/**
 * @brief Write a HyperRAM configuration to hardware.
 *
 * Applies a complete configuration to the HyperBus peripheral by writing all
 * configuration registers.
 *
 * @param[in] base Base address of the HyperBus peripheral
 * @param[in] cfg  Pointer to the configuration structure to apply
 * @return    0 on success, non-zero error code on failure
 */
int hyperram_cfg_write(void *base, const hyperram_cfg_t *cfg);

/**
 * @brief Print the current HyperRAM configuration to output.
 *
 * Reads the hardware configuration and displays all registers and settings in a
 * human-readable format.
 *
 * @param[in] base Base address of the HyperBus peripheral
 */
void hyperram_cfg_print(void *base);

#endif // HYPERBUS_H

/** @} */ // End of drivers_hyperbus group