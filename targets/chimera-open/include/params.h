// Copyright 2022 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

/**
 * @defgroup targets_chimera_host Chimera-Host Target
 * @brief Target-specific definitions and interfaces for Chimera-Host.
 * @{
 */

/**
 * @defgroup targets_chimera_host_memory Chimera-Host Memory Definitions
 * @ingroup targets_chimera_host
 * @brief Chimera-Host memory and address definitions.
 *
 * This header provides memory-mapped base addresses and boot configuration
 * parameters for Chimera-Host. In the future, these values should be
 * automatically generated as part of the SoC generation process.
 *
 * @{
 *
 * @author Nicole Narr
 * @author Christopher Reinwardt
 * @author Paul Scheffler
 * @author Viviane Potocnik vivianep@iis.ee.ethz.ch
 * @date 2025-02-05
 */

#pragma once

#include <stdint.h>

/**
 * @name Base Addresses (Provided at Link Time)
 * @brief These addresses are defined by the hardware platform.
 * @{
 */
extern void *__base_bootrom;  /**< Base address of Boot ROM. */
extern void *__base_regs;     /**< Base address of system registers. */
extern void *__base_llc;      /**< Base address of LLC. */
extern void *__base_uart;     /**< Base address of UART peripheral. */
extern void *__base_i2c;      /**< Base address of I2C peripheral. */
extern void *__base_spih;     /**< Base address of SPI Host peripheral. */
extern void *__base_gpio;     /**< Base address of GPIO peripheral. */
extern void *__base_slink;    /**< Base address of S-Link. */
extern void *__base_vga;      /**< Base address of VGA frame buffer. */
extern void *__base_clint;    /**< Base address of Core-Local Interruptor (CLINT). */
extern void *__base_plic;     /**< Base address of Platform-Level Interrupt Controller (PLIC). */
extern void *__base_dma;      /**< Base address of DMA controller. */
extern void *__base_axirt;    /**< Base address of AXI Router. */
extern void *__base_axirtgrd; /**< Base address of AXI Router Guard. */
extern void *__base_bus_err;  /**< Base address of Bus Error Register. */
extern void *__base_clic;     /**< Base address of Core-Level Interrupt Controller (CLIC). */
extern void *__base_usb;      /**< Base address of USB peripheral. */
extern void *__base_spm;      /**< Base address of Scratchpad Memory (SPM). */
extern void *__base_dram;     /**< Base address of DRAM. */
/** @} */

/**
 * @name Boot Configuration Parameters
 * @brief Defines constants related to booting the Chimera-Host.
 * @{
 */

/** @brief Default boot UART baud rate. */
static const uint32_t __BOOT_BAUDRATE = 115200;

/** @brief Maximum number of LBAs to copy to SPM for boot (48 KiB). */
static const uint64_t __BOOT_SPM_MAX_LBAS = 2 * 48;

/** @brief Location of the payload device tree. */
static void *const __BOOT_ZSL_DTB = (void *)0x80800000;

/** @brief Location of the firmware payload. */
static void *const __BOOT_ZSL_FW = (void *)0x80000000;

/** @brief GUID of the zero-stage loader partition from which we boot. */
static const uint64_t __BOOT_ZSL_TYPE_GUID[2] = {0x4CE4FD950269B26AUL, 0x622C41011494CF98UL};

/** @brief GUID of the flattened device tree blob (DTB). */
static const uint64_t __BOOT_DTB_TYPE_GUID[2] = {0x42DE2AEFBA442F61UL, 0x9DCB3A5DD7E43392UL};

/** @brief GUID of the firmware partition we boot into. */
static const uint64_t __BOOT_FW_TYPE_GUID[2] = {0x4B0D3F5B99EC86DAUL, 0x59F8A5CFBAC44B8FUL};
/** @} */

/** @} */ // End of targets_chimera_host_memory group
/** @} */ // End of targets_chimera_host group
