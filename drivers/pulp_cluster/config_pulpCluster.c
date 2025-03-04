// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Sergio Mazzola <smazzola@iis.ee.ethz.ch>

#include "soc.h"
#include "config_pulpCluster.h"

#include "addr_map.h"

#include <stdint.h>
#include <stddef.h>

/** \addtogroup cluster
 *  @{
 */

/**
 * @brief Configure PULP Cluster before usage.
 * This function performs different configurations of the PULP Cluster before offloading a
 * workloads. Namely, it:
 * - resets the busy register
 * - resets the return register
 *
 * @param cluster_id ID of the cluster to wait for.
 */
void config_pulpCluster(uint8_t cluster_id) {
    volatile uint32_t *busyRegAddr;
    volatile int32_t *returnRegAddr;

    // Find busy register address
    if (cluster_id == 0) {
        busyRegAddr = (volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_BUSY_REG_OFFSET);
    } else if (cluster_id == 1) {
        busyRegAddr = (volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_1_BUSY_REG_OFFSET);
    } else if (cluster_id == 2) {
        busyRegAddr = (volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_2_BUSY_REG_OFFSET);
    } else if (cluster_id == 3) {
        busyRegAddr = (volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_3_BUSY_REG_OFFSET);
    } else if (cluster_id == 4) {
        busyRegAddr = (volatile uint32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_4_BUSY_REG_OFFSET);
    }
    // Reset busy register
    *busyRegAddr = 0;

    // Find return register address
    if (cluster_id == 0) {
        returnRegAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_RETURN_REG_OFFSET);
    } else if (cluster_id == 1) {
        returnRegAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_1_RETURN_REG_OFFSET);
    } else if (cluster_id == 2) {
        returnRegAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_2_RETURN_REG_OFFSET);
    } else if (cluster_id == 3) {
        returnRegAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_3_RETURN_REG_OFFSET);
    } else if (cluster_id == 4) {
        returnRegAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_4_RETURN_REG_OFFSET);
    }
    // Reset return register
    *returnRegAddr = 0;
}

/** @}*/
