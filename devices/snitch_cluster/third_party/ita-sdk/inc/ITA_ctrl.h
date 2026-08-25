// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _ITA_CTRL_INCLUDE_GUARD_
#define _ITA_CTRL_INCLUDE_GUARD_

// Include Standard Libraries
#include <stdint.h>

// Include Application Headers
#include "ITA_mem.h"
#include "ITA_types.h"

// Include Target Specific Headers
#include "soc.h"
#include "cluster_4.h"

// Include Driver Headers

// Include Runtime Headers
#include "snrt.h"

// WIESEP: This part should go into a cluster specific SDK

#define ITA_TILES(s, e, p) (s | (e << 4) | (p << 8))
#define ITA_LAYER(layer, activation) ((layer) | ((activation) << 2))
#define ITA_FLAGS(weight_preload, weight_nextload, bias_disable, bias_direction, output_disable) \
    ((weight_preload) | ((weight_nextload) << 1) | ((bias_disable) << 2) | \
     ((bias_direction) << 3) | ((output_disable) << 4))

static inline void __attribute((always_inline))
ita_set_addresses(uint32_t input_addr, uint32_t weight_addr, uint32_t weight_next_addr,
                  uint32_t bias_addr, uint32_t output_addr) {
    // Program ITA
    if (input_addr > CLUSTER_4_BASE) {
        *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x20) = input_addr - CLUSTER_4_BASE;
    }
    if (weight_addr > CLUSTER_4_BASE) {
        *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x24) = weight_addr - CLUSTER_4_BASE;
    }
    if (weight_next_addr > CLUSTER_4_BASE) {
        *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x28) =
            weight_next_addr - CLUSTER_4_BASE;
    }
    if (bias_addr > CLUSTER_4_BASE) {
        *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x2C) = bias_addr - CLUSTER_4_BASE;
    }
    if (output_addr > CLUSTER_4_BASE) {
        *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x30) = output_addr - CLUSTER_4_BASE;
    }
}

static inline void __attribute((always_inline)) ita_set_tiles(uint32_t tiles) {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x38) = tiles;
}

static inline void __attribute((always_inline)) ita_set_layers(uint32_t layer) {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x54) = layer; // ctrl engine
}

static inline void __attribute((always_inline)) ita_set_flags(uint32_t flags) {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x58) = flags; // ctrl stream
}

static inline void __attribute((always_inline)) ita_write_rqs_params(uint32_t eps1, uint32_t eps2,
                                                                     uint32_t right_shift1,
                                                                     uint32_t right_shift2,
                                                                     uint32_t add1, uint32_t add2) {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x3C) = eps1;
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x40) = eps2;
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x44) = right_shift1;
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x48) = right_shift2;
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x4C) = add1;
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x50) = add2;
}

static inline void __attribute((always_inline)) ita_soft_clear() {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x14) = 0;
    for (volatile int i = 0; i < 10; i++);
}

static inline void __attribute((always_inline)) ita_soft_clear_keep_regs() {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x14) = 1;
    for (volatile uint32_t i = 0; i < 10; i++);
}

static inline void __attribute((always_inline)) ita_acquire_job() {
    while (*(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x04) < 1);
}

static inline void __attribute((always_inline)) ita_wait_job() {
    // Do some dummy operaitons to avoid busy waiting too tightly
    // for (volatile int i = 0; i < 10; i++);
    while (*(volatile uint32_t *)(CLUSTER_4_HWPE_BUSY_ADDR) != 0);
}

static inline void __attribute((always_inline)) ita_trigger() {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x00) = 0;
}

static inline void __attribute((always_inline)) ita_commit() {
    *(volatile uint32_t *)(CLUSTER_4_HWPE_ITA_BASE_ADDR + 0x00) = 1;
}

#endif //_ITA_CTRL_INCLUDE_GUARD_
