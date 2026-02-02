// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef CHIMERA_PADFRAME_H
#define CHIMERA_PADFRAME_H
#include <stdint.h>

#ifndef CHIMERA_PADFRAME_BASE_ADDRESS
#define CHIMERA_PADFRAME_BASE_ADDRESS 0x30002000
#endif

/**
 * Sets the chip2pad pad signal for the pad: gpio_0
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_0_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_0
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_0_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_0
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_0_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_0
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_0_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_0
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_0_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_0
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_0_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_0
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_0_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_0
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_0_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_0
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_0_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_0
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_0_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_0
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_0_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_0
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_0_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_0
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_0_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_0
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_0_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_0
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_0_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_0
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_0_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_0_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_0_group_GPIOA_port_GPIO0 = 1,
    CHIMERA_PADFRAME_AON_GPIO_0_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_0_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_0_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_0_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_0_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_0_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_0_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_0_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_0_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_0_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_0_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_0_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_0.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_0_mux_set(chimera_padframe_aon_gpio_0_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_0.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_0_mux_sel_t chimera_padframe_aon_gpio_0_mux_get();

/**
 * Sets the chip2pad pad signal for the pad: gpio_1
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_1_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_1
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_1_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_1
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_1_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_1
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_1_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_1
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_1_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_1
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_1_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_1
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_1_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_1
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_1_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_1
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_1_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_1
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_1_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_1
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_1_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_1
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_1_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_1
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_1_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_1
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_1_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_1
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_1_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_1
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_1_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_1_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_1_group_GPIOA_port_GPIO1 = 1,
    CHIMERA_PADFRAME_AON_GPIO_1_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_1_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_1_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_1_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_1_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_1_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_1_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_1_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_1_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_1_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_1_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_1_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_1.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_1_mux_set(chimera_padframe_aon_gpio_1_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_1.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_1_mux_sel_t chimera_padframe_aon_gpio_1_mux_get();

/**
 * Sets the chip2pad pad signal for the pad: gpio_2
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_2_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_2
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_2_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_2
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_2_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_2
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_2_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_2
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_2_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_2
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_2_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_2
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_2_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_2
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_2_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_2
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_2_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_2
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_2_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_2
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_2_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_2
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_2_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_2
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_2_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_2
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_2_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_2
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_2_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_2
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_2_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_2_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_2_group_GPIOA_port_GPIO2 = 1,
    CHIMERA_PADFRAME_AON_GPIO_2_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_2_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_2_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_2_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_2_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_2_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_2_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_2_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_2_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_2_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_2_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_2_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_2.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_2_mux_set(chimera_padframe_aon_gpio_2_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_2.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_2_mux_sel_t chimera_padframe_aon_gpio_2_mux_get();

/**
 * Sets the chip2pad pad signal for the pad: gpio_3
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_3_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_3
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_3_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_3
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_3_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_3
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_3_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_3
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_3_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_3
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_3_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_3
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_3_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_3
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_3_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_3
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_3_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_3
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_3_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_3
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_3_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_3
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_3_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_3
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_3_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_3
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_3_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_3
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_3_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_3
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_3_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_3_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_3_group_GPIOA_port_GPIO3 = 1,
    CHIMERA_PADFRAME_AON_GPIO_3_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_3_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_3_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_3_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_3_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_3_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_3_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_3_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_3_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_3_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_3_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_3_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_3.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_3_mux_set(chimera_padframe_aon_gpio_3_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_3.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_3_mux_sel_t chimera_padframe_aon_gpio_3_mux_get();

/**
 * Sets the chip2pad pad signal for the pad: gpio_4
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_4_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_4
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_4_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_4
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_4_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_4
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_4_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_4
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_4_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_4
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_4_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_4
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_4_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_4
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_4_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_4
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_4_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_4
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_4_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_4
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_4_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_4
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_4_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_4
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_4_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_4
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_4_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_4
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_4_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_4
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_4_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_4_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_4_group_GPIOA_port_GPIO4 = 1,
    CHIMERA_PADFRAME_AON_GPIO_4_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_4_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_4_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_4_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_4_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_4_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_4_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_4_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_4_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_4_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_4_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_4_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_4.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_4_mux_set(chimera_padframe_aon_gpio_4_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_4.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_4_mux_sel_t chimera_padframe_aon_gpio_4_mux_get();

/**
 * Sets the chip2pad pad signal for the pad: gpio_5
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_5_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_5
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_5_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_5
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_5_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_5
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_5_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_5
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_5_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_5
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_5_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_5
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_5_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_5
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_5_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_5
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_5_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_5
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_5_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_5
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_5_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_5
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_5_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_5
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_5_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_5
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_5_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_5
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_5_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_5
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_5_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_5_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_5_group_GPIOA_port_GPIO5 = 1,
    CHIMERA_PADFRAME_AON_GPIO_5_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_5_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_5_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_5_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_5_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_5_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_5_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_5_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_5_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_5_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_5_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_5_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_5.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_5_mux_set(chimera_padframe_aon_gpio_5_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_5.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_5_mux_sel_t chimera_padframe_aon_gpio_5_mux_get();

/**
 * Sets the chip2pad pad signal for the pad: gpio_6
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_6_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_6
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_6_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_6
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_6_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_6
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_6_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_6
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_6_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_6
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_6_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_6
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_6_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_6
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_6_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_6
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_6_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_6
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_6_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_6
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_6_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_6
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_6_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_6
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_6_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_6
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_6_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_6
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_6_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_6
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_6_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_6_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_6_group_GPIOA_port_GPIO6 = 1,
    CHIMERA_PADFRAME_AON_GPIO_6_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_6_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_6_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_6_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_6_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_6_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_6_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_6_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_6_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_6_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_6_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_6_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_6.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_6_mux_set(chimera_padframe_aon_gpio_6_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_6.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_6_mux_sel_t chimera_padframe_aon_gpio_6_mux_get();

/**
 * Sets the chip2pad pad signal for the pad: gpio_7
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_7_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_7
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_7_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_7
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_7_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_7
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_7_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_7
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_7_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_7
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_7_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_7
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_7_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_7
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_7_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_7
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_7_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_7
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_7_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_7
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_7_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_7
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_7_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_7
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_7_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_7
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_7_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_7
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_7_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_7
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_7_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_7_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_7_group_GPIOA_port_GPIO7 = 1,
    CHIMERA_PADFRAME_AON_GPIO_7_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_7_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_7_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_7_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_7_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_7_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_7_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_7_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_7_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_7_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_7_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_7_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_7.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_7_mux_set(chimera_padframe_aon_gpio_7_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_7.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_7_mux_sel_t chimera_padframe_aon_gpio_7_mux_get();

/**
 * Sets the chip2pad pad signal for the pad: gpio_8
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_8_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_8
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_8_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_8
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_8_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_8
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_8_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_8
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_8_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_8
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_8_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_8
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_8_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_8
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_8_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_8
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_8_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_8
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_8_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_8
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_8_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_8
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_8_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_8
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_8_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_8
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_8_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_8
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_8_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_8
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_8_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_8_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_8_group_GPIOA_port_GPIO8 = 1,
    CHIMERA_PADFRAME_AON_GPIO_8_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_8_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_8_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_8_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_8_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_8_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_8_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_8_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_8_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_8_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_8_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_8_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_8.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_8_mux_set(chimera_padframe_aon_gpio_8_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_8.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_8_mux_sel_t chimera_padframe_aon_gpio_8_mux_get();

/**
 * Sets the chip2pad pad signal for the pad: gpio_9
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_9_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_9
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_9_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_9
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_9_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_9
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_9_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_9
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_9_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_9
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_9_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_9
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_9_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_9
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_9_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_9
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_9_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_9
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_9_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_9
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_9_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_9
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_9_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_9
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_9_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_9
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_9_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_9
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_9_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_9
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_9_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_9_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_9_group_GPIOA_port_GPIO9 = 1,
    CHIMERA_PADFRAME_AON_GPIO_9_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_9_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_9_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_9_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_9_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_9_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_9_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_9_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_9_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_9_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_9_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_9_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_9.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_9_mux_set(chimera_padframe_aon_gpio_9_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_9.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_9_mux_sel_t chimera_padframe_aon_gpio_9_mux_get();

/**
 * Sets the chip2pad pad signal for the pad: gpio_10
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_10_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_10
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_10_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_10
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_10_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_10
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_10_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_10
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_10_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_10
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_10_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_10
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_10_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_10
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_10_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_10
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_10_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_10
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_10_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_10
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_10_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_10
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_10_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_10
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_10_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_10
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_10_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_10
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_10_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_10
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_10_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_10_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_10_group_GPIOA_port_GPIO10 = 1,
    CHIMERA_PADFRAME_AON_GPIO_10_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_10_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_10_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_10_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_10_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_10_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_10_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_10_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_10_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_10_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_10_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_10_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_10.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_10_mux_set(chimera_padframe_aon_gpio_10_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_10.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_10_mux_sel_t chimera_padframe_aon_gpio_10_mux_get();

/**
 * Sets the chip2pad pad signal for the pad: gpio_11
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_11_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_11
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_11_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_11
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_11_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_11
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_11_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_11
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_11_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_11
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_11_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_11
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_11_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_11
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_11_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_11
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_11_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_11
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_11_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_11
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_11_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_11
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_11_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_11
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_11_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_11
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_11_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_11
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_11_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_11
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_11_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_11_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_11_group_GPIOA_port_GPIO11 = 1,
    CHIMERA_PADFRAME_AON_GPIO_11_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_11_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_11_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_11_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_11_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_11_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_11_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_11_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_11_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_11_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_11_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_11_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_11.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_11_mux_set(chimera_padframe_aon_gpio_11_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_11.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_11_mux_sel_t chimera_padframe_aon_gpio_11_mux_get();

/**
 * Sets the chip2pad pad signal for the pad: gpio_12
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_12_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_12
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_12_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_12
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_12_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_12
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_12_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_12
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_12_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_12
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_12_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_12
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_12_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_12
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_12_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_12
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_12_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_12
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_12_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_12
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_12_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_12
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_12_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_12
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_12_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_12
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_12_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_12
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_12_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_12
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_12_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_12_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_12_group_GPIOA_port_GPIO12 = 1,
    CHIMERA_PADFRAME_AON_GPIO_12_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_12_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_12_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_12_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_12_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_12_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_12_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_12_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_12_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_12_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_12_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_12_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_12.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_12_mux_set(chimera_padframe_aon_gpio_12_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_12.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_12_mux_sel_t chimera_padframe_aon_gpio_12_mux_get();

/**
 * Sets the chip2pad pad signal for the pad: gpio_13
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_13_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_13
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_13_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_13
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_13_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_13
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_13_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_13
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_13_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_13
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_13_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_13
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_13_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_13
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_13_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_13
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_13_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_13
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_13_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_13
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_13_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_13
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_13_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_13
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_13_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_13
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_13_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_13
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_13_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_13
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_13_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_13_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_13_group_GPIOA_port_GPIO13 = 1,
    CHIMERA_PADFRAME_AON_GPIO_13_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_13_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_13_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_13_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_13_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_13_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_13_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_13_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_13_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_13_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_13_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_13_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_13.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_13_mux_set(chimera_padframe_aon_gpio_13_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_13.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_13_mux_sel_t chimera_padframe_aon_gpio_13_mux_get();

/**
 * Sets the chip2pad pad signal for the pad: gpio_14
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_14_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_14
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_14_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_14
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_14_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_14
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_14_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_14
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_14_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_14
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_14_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_14
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_14_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_14
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_14_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_14
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_14_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_14
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_14_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_14
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_14_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_14
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_14_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_14
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_14_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_14
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_14_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_14
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_14_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_14
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_14_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_14_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_14_group_GPIOA_port_GPIO14 = 1,
    CHIMERA_PADFRAME_AON_GPIO_14_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_14_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_14_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_14_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_14_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_14_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_14_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_14_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_14_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_14_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_14_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_14_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_14.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_14_mux_set(chimera_padframe_aon_gpio_14_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_14.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_14_mux_sel_t chimera_padframe_aon_gpio_14_mux_get();

/**
 * Sets the chip2pad pad signal for the pad: gpio_15
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_15_cfg_chip2pad_set(uint8_t value);

/**
 * Get the currently configured chip2pad value for the pad: gpio_15
 *
 * @return The value of the chip2pad field
 */
uint8_t chimera_padframe_aon_gpio_15_cfg_chip2pad_get();

/**
 * Sets the drv pad signal for the pad: gpio_15
 *
 * @param value The value to program into the pad configuration register. A value smaller than 3.
 */
void chimera_padframe_aon_gpio_15_cfg_drv_set(uint8_t value);

/**
 * Get the currently configured drv value for the pad: gpio_15
 *
 * @return The value of the drv field
 */
uint8_t chimera_padframe_aon_gpio_15_cfg_drv_get();

/**
 * Sets the pde pad signal for the pad: gpio_15
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_15_cfg_pde_set(uint8_t value);

/**
 * Get the currently configured pde value for the pad: gpio_15
 *
 * @return The value of the pde field
 */
uint8_t chimera_padframe_aon_gpio_15_cfg_pde_get();

/**
 * Sets the pue pad signal for the pad: gpio_15
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_15_cfg_pue_set(uint8_t value);

/**
 * Get the currently configured pue value for the pad: gpio_15
 *
 * @return The value of the pue field
 */
uint8_t chimera_padframe_aon_gpio_15_cfg_pue_get();

/**
 * Sets the rxe pad signal for the pad: gpio_15
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_15_cfg_rxe_set(uint8_t value);

/**
 * Get the currently configured rxe value for the pad: gpio_15
 *
 * @return The value of the rxe field
 */
uint8_t chimera_padframe_aon_gpio_15_cfg_rxe_get();

/**
 * Sets the slw pad signal for the pad: gpio_15
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_15_cfg_slw_set(uint8_t value);

/**
 * Get the currently configured slw value for the pad: gpio_15
 *
 * @return The value of the slw field
 */
uint8_t chimera_padframe_aon_gpio_15_cfg_slw_get();

/**
 * Sets the smt pad signal for the pad: gpio_15
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_15_cfg_smt_set(uint8_t value);

/**
 * Get the currently configured smt value for the pad: gpio_15
 *
 * @return The value of the smt field
 */
uint8_t chimera_padframe_aon_gpio_15_cfg_smt_get();

/**
 * Sets the trie pad signal for the pad: gpio_15
 *
 * @param value The value to program into the pad configuration register. A value smaller than 1.
 */
void chimera_padframe_aon_gpio_15_cfg_trie_set(uint8_t value);

/**
 * Get the currently configured trie value for the pad: gpio_15
 *
 * @return The value of the trie field
 */
uint8_t chimera_padframe_aon_gpio_15_cfg_trie_get();

typedef enum {
    CHIMERA_PADFRAME_AON_GPIO_15_REGISTER = 0,
    CHIMERA_PADFRAME_AON_GPIO_15_group_GPIOA_port_GPIO15 = 1,
    CHIMERA_PADFRAME_AON_GPIO_15_group_I2C0_port_SCL = 2,
    CHIMERA_PADFRAME_AON_GPIO_15_group_I2C0_port_SDA = 3,
    CHIMERA_PADFRAME_AON_GPIO_15_group_QSPI0_port_CSB0 = 4,
    CHIMERA_PADFRAME_AON_GPIO_15_group_QSPI0_port_CSB1 = 5,
    CHIMERA_PADFRAME_AON_GPIO_15_group_QSPI0_port_SCK = 6,
    CHIMERA_PADFRAME_AON_GPIO_15_group_QSPI0_port_SD0 = 7,
    CHIMERA_PADFRAME_AON_GPIO_15_group_QSPI0_port_SD1 = 8,
    CHIMERA_PADFRAME_AON_GPIO_15_group_QSPI0_port_SD2 = 9,
    CHIMERA_PADFRAME_AON_GPIO_15_group_QSPI0_port_SD3 = 10,
    CHIMERA_PADFRAME_AON_GPIO_15_group_UART0_port_RX = 11,
    CHIMERA_PADFRAME_AON_GPIO_15_group_UART0_port_TX = 12,
} chimera_padframe_aon_gpio_15_mux_sel_t;

/**
 * Choose the entity (a port or the dedicated configuration register) that controls gpio_15.
 *
 * @param mux_sel Port or configuration register to connect to the pad.
 */
void chimera_padframe_aon_gpio_15_mux_set(chimera_padframe_aon_gpio_15_mux_sel_t mux_sel);

/**
 * Read the current multiplexer select value configured for gpio_15.
 *
 * @return Port or configuration register currently connected to the pad.
 */
chimera_padframe_aon_gpio_15_mux_sel_t chimera_padframe_aon_gpio_15_mux_get();

#endif /*  CHIMERA_PADFRAME_H */
