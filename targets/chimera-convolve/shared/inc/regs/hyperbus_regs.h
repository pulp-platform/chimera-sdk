// Copyright 2025 ETH Zurich and University of Bologna.
// SPDX-License-Identifier: SHL-0.51

#ifndef _HYPERBUS_REG_DEFS_
#define _HYPERBUS_REG_DEFS_

#ifdef __cplusplus
extern "C" {
#endif

// Register width in bits
#define HYPERBUS_PARAM_REG_WIDTH 32

// Number of fixed (non-chip) configuration registers in hyperbus_cfg_regs
#define HYPERBUS_PARAM_NUM_BASE_REGS 11

// ----------------------------------------------------------------------------
// Base configuration register offsets (word-stride, i.e., +0x4 per register)
// ----------------------------------------------------------------------------

// [0] t_latency_access (cfg.t_latency_access, [3:0])
#define HYPERBUS_T_LATENCY_ACCESS_REG_OFFSET 0x0
#define HYPERBUS_T_LATENCY_ACCESS_MASK 0x0000000f
#define HYPERBUS_T_LATENCY_ACCESS_OFFSET 0
#define HYPERBUS_T_LATENCY_ACCESS_FIELD \
    ((bitfield_field32_t){.mask = HYPERBUS_T_LATENCY_ACCESS_MASK, \
                          .index = HYPERBUS_T_LATENCY_ACCESS_OFFSET})

// [1] en_latency_additional (cfg.en_latency_additional, bit 0)
#define HYPERBUS_EN_LATENCY_ADDITIONAL_REG_OFFSET 0x4
#define HYPERBUS_EN_LATENCY_ADDITIONAL_MASK 0x00000001
#define HYPERBUS_EN_LATENCY_ADDITIONAL_OFFSET 0
#define HYPERBUS_EN_LATENCY_ADDITIONAL_FIELD \
    ((bitfield_field32_t){.mask = HYPERBUS_EN_LATENCY_ADDITIONAL_MASK, \
                          .index = HYPERBUS_EN_LATENCY_ADDITIONAL_OFFSET})

// [2] t_burst_max (cfg.t_burst_max, [15:0])
#define HYPERBUS_T_BURST_MAX_REG_OFFSET 0x8
#define HYPERBUS_T_BURST_MAX_MASK 0x0000ffff
#define HYPERBUS_T_BURST_MAX_OFFSET 0
#define HYPERBUS_T_BURST_MAX_FIELD \
    ((bitfield_field32_t){.mask = HYPERBUS_T_BURST_MAX_MASK, .index = HYPERBUS_T_BURST_MAX_OFFSET})

// [3] t_read_write_recovery (cfg.t_read_write_recovery, [3:0])
#define HYPERBUS_T_READ_WRITE_RECOVERY_REG_OFFSET 0xc
#define HYPERBUS_T_READ_WRITE_RECOVERY_MASK 0x0000000f
#define HYPERBUS_T_READ_WRITE_RECOVERY_OFFSET 0
#define HYPERBUS_T_READ_WRITE_RECOVERY_FIELD \
    ((bitfield_field32_t){.mask = HYPERBUS_T_READ_WRITE_RECOVERY_MASK, \
                          .index = HYPERBUS_T_READ_WRITE_RECOVERY_OFFSET})

// [4] t_rx_clk_delay (cfg.t_rx_clk_delay, [3:0])
#define HYPERBUS_T_RX_CLK_DELAY_REG_OFFSET 0x10
#define HYPERBUS_T_RX_CLK_DELAY_MASK 0x0000000f
#define HYPERBUS_T_RX_CLK_DELAY_OFFSET 0
#define HYPERBUS_T_RX_CLK_DELAY_FIELD \
    ((bitfield_field32_t){.mask = HYPERBUS_T_RX_CLK_DELAY_MASK, \
                          .index = HYPERBUS_T_RX_CLK_DELAY_OFFSET})

// [5] t_tx_clk_delay (cfg.t_tx_clk_delay, [3:0])
#define HYPERBUS_T_TX_CLK_DELAY_REG_OFFSET 0x14
#define HYPERBUS_T_TX_CLK_DELAY_MASK 0x0000000f
#define HYPERBUS_T_TX_CLK_DELAY_OFFSET 0
#define HYPERBUS_T_TX_CLK_DELAY_FIELD \
    ((bitfield_field32_t){.mask = HYPERBUS_T_TX_CLK_DELAY_MASK, \
                          .index = HYPERBUS_T_TX_CLK_DELAY_OFFSET})

// [6] address_mask_msb (cfg.address_mask_msb, [4:0])
#define HYPERBUS_ADDRESS_MASK_MSB_REG_OFFSET 0x18
#define HYPERBUS_ADDRESS_MASK_MSB_MASK 0x0000001f
#define HYPERBUS_ADDRESS_MASK_MSB_OFFSET 0
#define HYPERBUS_ADDRESS_MASK_MSB_FIELD \
    ((bitfield_field32_t){.mask = HYPERBUS_ADDRESS_MASK_MSB_MASK, \
                          .index = HYPERBUS_ADDRESS_MASK_MSB_OFFSET})

// [7] address_space (cfg.address_space, bit 0)
#define HYPERBUS_ADDRESS_SPACE_REG_OFFSET 0x1c
#define HYPERBUS_ADDRESS_SPACE_MASK 0x00000001
#define HYPERBUS_ADDRESS_SPACE_OFFSET 0
#define HYPERBUS_ADDRESS_SPACE_FIELD \
    ((bitfield_field32_t){.mask = HYPERBUS_ADDRESS_SPACE_MASK, \
                          .index = HYPERBUS_ADDRESS_SPACE_OFFSET})

// [8] phys_in_use (cfg.phys_in_use, bit 0)
#define HYPERBUS_PHYS_IN_USE_REG_OFFSET 0x20
#define HYPERBUS_PHYS_IN_USE_MASK 0x00000001
#define HYPERBUS_PHYS_IN_USE_OFFSET 0
#define HYPERBUS_PHYS_IN_USE_FIELD \
    ((bitfield_field32_t){.mask = HYPERBUS_PHYS_IN_USE_MASK, .index = HYPERBUS_PHYS_IN_USE_OFFSET})

// [9] which_phy (cfg.which_phy, bit 0)
#define HYPERBUS_WHICH_PHY_REG_OFFSET 0x24
#define HYPERBUS_WHICH_PHY_MASK 0x00000001
#define HYPERBUS_WHICH_PHY_OFFSET 0
#define HYPERBUS_WHICH_PHY_FIELD \
    ((bitfield_field32_t){.mask = HYPERBUS_WHICH_PHY_MASK, .index = HYPERBUS_WHICH_PHY_OFFSET})

// [10] t_csh_cycles (cfg.t_csh_cycles, [3:0])
#define HYPERBUS_T_CSH_CYCLES_REG_OFFSET 0x28
#define HYPERBUS_T_CSH_CYCLES_MASK 0x0000000f
#define HYPERBUS_T_CSH_CYCLES_OFFSET 0
#define HYPERBUS_T_CSH_CYCLES_FIELD \
    ((bitfield_field32_t){.mask = HYPERBUS_T_CSH_CYCLES_MASK, \
                          .index = HYPERBUS_T_CSH_CYCLES_OFFSET})

// ----------------------------------------------------------------------------
// Per-chip address range registers (crange[chip][0/1])
//
// In RTL:
//   sel_reg in [NumBaseRegs .. NumBaseRegs + 2*NumChips - 1]
//   {sel_chip, chip_reg} = sel_reg - NumBaseRegs;
//   chip_reg=0 -> start, chip_reg=1 -> end (end noninclusive)
//
// These macros compute offsets for a given chip index.
// ----------------------------------------------------------------------------

#define HYPERBUS_CHIP_RANGE_BASE_OFFSET (HYPERBUS_PARAM_NUM_BASE_REGS * 4u)
#define HYPERBUS_CHIP_RANGE_STRIDE_BYTES 8u

// Start address register for chip `chip` (crange[chip][0])
#define HYPERBUS_CHIP_START_REG_OFFSET(chip) \
    (HYPERBUS_CHIP_RANGE_BASE_OFFSET + ((uint32_t)(chip) * HYPERBUS_CHIP_RANGE_STRIDE_BYTES) + 0u)

// End address register for chip `chip` (crange[chip][1]) - end is noninclusive
#define HYPERBUS_CHIP_END_REG_OFFSET(chip) \
    (HYPERBUS_CHIP_RANGE_BASE_OFFSET + ((uint32_t)(chip) * HYPERBUS_CHIP_RANGE_STRIDE_BYTES) + 4u)

// Full 32-bit value (base/end are stored as the full register value)
#define HYPERBUS_CHIP_RANGE_ADDR_MASK 0xffffffffu
#define HYPERBUS_CHIP_RANGE_ADDR_OFFSET 0
#define HYPERBUS_CHIP_RANGE_ADDR_FIELD \
    ((bitfield_field32_t){.mask = HYPERBUS_CHIP_RANGE_ADDR_MASK, \
                          .index = HYPERBUS_CHIP_RANGE_ADDR_OFFSET})

#ifdef __cplusplus
} // extern "C"
#endif
#endif // _HYPERBUS_REG_DEFS_
       // End generated register defines for hyperbus
