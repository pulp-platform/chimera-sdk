// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Moritz Scherer <scheremo@iis.ee.ethz.ch>

#include "soc.h"
#include "driver.h"

#include <stddef.h>

#define TESTVAL 0x050CCE55
#define STACK_ADDRESS (CLUSTER_4_BASE + 0x20000 - 8)

static uint32_t *clintPointer = (uint32_t *)CLINT_CTRL_BASE;

void clusterInterruptHandler() {
    uint8_t hartId;
    asm("csrr %0, mhartid" : "=r"(hartId)::);

    volatile uint32_t *interruptTarget = clintPointer + hartId;
    *interruptTarget = 0;
    return;
}

typedef struct {
    int value;
} offloadArgs_t;

static offloadArgs_t offloadArgs = {.value = 0xdeadbeef};

int32_t testReturn(void *args) {
    // Cast to the correct struct
    offloadArgs_t *argsStruct = (offloadArgs_t *)args;

    // Check if the value is correct
    if (argsStruct->value != 0xdeadbeef) {
        return -1;
    }

    return TESTVAL;
}

int main() {
    setup_snitchCluster_interruptHandler(clusterInterruptHandler);
    offload_snitchCluster_core(testReturn, &offloadArgs, (void *)(STACK_ADDRESS), 4, 0);
    uint32_t retVal = wait_snitchCluster_return(4);

    return (retVal != (TESTVAL | 0x000000001));
}