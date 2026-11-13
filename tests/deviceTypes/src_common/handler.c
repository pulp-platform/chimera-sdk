// SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#include "test_cluster.h"

#include "soc.h"
#include "trampoline.h"

// Clears this hart's interrupt flag; stack, thread and global pointer may not be set up yet
__attribute__((naked)) void clusterInterruptHandler() {
    _SET_CLUSTER_BUSY();
    _SETUP_GP();

    asm volatile("csrr t0, mhartid\n"
                 "la t1, __base_clint\n"
                 "slli t0, t0, 2\n"
                 "add t1, t1, t0\n"
                 "sw zero, 0(t1)\n"
                 "ret"
                 :
                 :
                 : "t0", "t1");
}
