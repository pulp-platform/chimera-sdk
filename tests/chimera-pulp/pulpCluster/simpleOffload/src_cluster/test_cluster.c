// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Moritz Scherer <scheremo@iis.ee.ethz.ch>
// Sergio Mazzola <smazzola@iis.ee.ethz.ch>

#include "test_cluster.h"
#include "test_host.h"

#include "soc.h"

#include "trampoline_pulpCluster.h"

/**
 * @brief Main function of the cluster test.
 *
 * @return int Return 0 if the test was successful, -1 otherwise.
 */
int32_t testReturn(void *args) {
    // Cast to the correct struct
    offloadArgs_t *argsStruct = (offloadArgs_t *)args;

    // Check if the value is correct
    if (argsStruct->value != 0xdeadbeef) {
        return -1;
    }

    return TESTVAL;
}