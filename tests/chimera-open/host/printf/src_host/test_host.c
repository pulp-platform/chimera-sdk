// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Victor Jung <jungvi@iis.ee.ethz.ch>

#include "uart.h"

#include <stdio.h>

int main(void) {

    volatile int a = 42;
    printf("%d\n", a);
    printf("CIAO\n");

    // JUNGVI: This is here to give enough time to the UART buffer to finish it's transaction
    for (int i = 0; i < 420; i++) {
        a += a * 5;
    }

    return 0;
}
