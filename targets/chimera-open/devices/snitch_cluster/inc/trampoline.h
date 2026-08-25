// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _CLUSTER_SNITCH_TRAMPOLINE_INCLUDE_GUARD_
#define _CLUSTER_SNITCH_TRAMPOLINE_INCLUDE_GUARD_

#include <stdint.h>

/** \addtogroup device_snitchCluster
 *  @{
 */

/**
 * @brief Setup the core to a known state.
 *
 * This function will set up the global pointer and thread pointer for the core.
 */
#define _SETUP_GP() \
    asm volatile(".option push\n" \
                 ".option norelax\n" \
                 "la gp, __global_pointer$\n" \
                 ".option pop\n" \
                 : /* No outputs */ \
                 : /* No inputs */ \
                 : /* No clobbered registers */)

/**
 * @brief Calculate the cluster ID from mhartid and set busy flag.
 *
 * @note This is a string fragment intended to be used inside an asm volatile block.
 * It reqires the following operands:
 *  %0: Base address of the busy register for cluster 0
 *  %1: Threshold for cluster 1
 *  %2: Threshold for cluster 2
 *  %3: Threshold for cluster 3
 *  %4: Threshold for cluster 4
 */
#define __CLUSTER_ID_ASM \
    "csrr t0, mhartid\n" /* t0 = mhartid */ \
    "li t1, 0\n"         /* t1 = cluster_id = 0 */ \
    "sltu t3, t0, %1\n"  /* t3 = (t0 < thresh1) ? 1 : 0 */ \
    "xori t3, t3, 1\n"   /* t3 = (t0 >= thresh1) ? 1 : 0 */ \
    "add t1, t1, t3\n"   /* t1 += t3 (cluster_id = 1) */ \
    "sltu t3, t0, %2\n"  /* t3 = (t0 < thresh2) ? 1 : 0 */ \
    "xori t3, t3, 1\n"   /* t3 = (t0 >= thresh2) ? 1 : 0 */ \
    "add t1, t1, t3\n"   /* t1 += t3 (cluster_id = 2) */ \
    "sltu t3, t0, %3\n"  /* t3 = (t0 < thresh3) ? 1 : 0 */ \
    "xori t3, t3, 1\n"   /* t3 = (t0 >= thresh3) ? 1 : 0 */ \
    "add t1, t1, t3\n"   /* t1 += t3 (cluster_id = 3) */ \
    "sltu t3, t0, %4\n"  /* t3 = (t0 < thresh4) ? 1 : 0 */ \
    "xori t3, t3, 1\n"   /* t3 = (t0 >= thresh4) ? 1 : 0 */ \
    "add t1, t1, t3\n"   /* t1 += t3 (cluster_id = 4) */

/**
 * @brief Calculate the cluster ID from mhartid and set the busy flag.
 *
 * @note This is a naked-friendly macro intended to be used inside an asm volatile block.
 */
#define _SET_CLUSTER_BUSY() \
    asm volatile(__CLUSTER_ID_ASM "slli t1, t1, 2\n" /* t1 = cluster_id * 4 */ \
                                  "add t1, %0, t1\n" /* t1 = base + cluster_id*4 */ \
                                  "li t2, 1\n" \
                                  "sw t2, 0(t1)\n" \
                 : /* no outputs */ \
                 : "r"((uintptr_t)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_BUSY_REG_OFFSET)), \
                   "r"((uintptr_t)(HOST_NUMCORES + CLUSTER_0_NUMCORES)), \
                   "r"((uintptr_t)(HOST_NUMCORES + CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES)), \
                   "r"((uintptr_t)(HOST_NUMCORES + CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES + \
                                   CLUSTER_2_NUMCORES)), \
                   "r"((uintptr_t)(HOST_NUMCORES + CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES + \
                                   CLUSTER_2_NUMCORES + CLUSTER_3_NUMCORES)) \
                 : "t0", "t1", "t2", "t3", "memory");

/**
 * @brief Compute cluster id from `mhartid` and clear busy.
 *
 * @note This is a naked-friendly macro intended to be used inside an asm volatile block.
 */
#define _CLEAR_CLUSTER_BUSY() \
    asm volatile(__CLUSTER_ID_ASM "slli t1, t1, 2\n" /* t1 = cluster_id * 4 */ \
                                  "add t1, %0, t1\n" /* t1 = base + cluster_id*4 */ \
                                  "li t2, 0\n" \
                                  "sw t2, 0(t1)\n" \
                 : /* no outputs */ \
                 : "r"((uintptr_t)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_BUSY_REG_OFFSET)), \
                   "r"((uintptr_t)(HOST_NUMCORES + CLUSTER_0_NUMCORES)), \
                   "r"((uintptr_t)(HOST_NUMCORES + CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES)), \
                   "r"((uintptr_t)(HOST_NUMCORES + CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES + \
                                   CLUSTER_2_NUMCORES)), \
                   "r"((uintptr_t)(HOST_NUMCORES + CLUSTER_0_NUMCORES + CLUSTER_1_NUMCORES + \
                                   CLUSTER_2_NUMCORES + CLUSTER_3_NUMCORES)) \
                 : "t0", "t1", "t2", "t3", "memory");

/** @} */

#endif //_CLUSTER_SNITCH_TRAMPOLINE_INCLUDE_GUARD_
