// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Simplified startup code for Snitch Cluster device (chimera-open target).
//
// ISA: rv32imafd_xdma — includes F and D extensions; FP registers are cleared.
//
// Execution model: The cluster cores are NOT started via a traditional main().
// The host programs CHIMERA_SNITCH_INTR_HANDLER_ADDR_REG with the address of
// clusterInterruptHandler and then sends a hardware interrupt to wake the cores.
// The hardware dispatch mechanism handles the jump to the handler; this startup
// code only initialises registers and parks cores in a WFI loop.
//
// NOTE: The global pointer (gp) and stack pointer (sp) are NOT initialised
// here.  The host programs the cluster's boot address and the trampoline sets
// up gp/sp before any C code runs.

.section .init
.global _start
_start:
    // Clear integer registers (except gp/sp set by host, and x0 which is
    // hardwired to zero)
    mv t0, x0
    mv t1, x0
    mv t2, x0
    mv t3, x0
    mv t4, x0
    mv t5, x0
    mv t6, x0
    mv a0, x0
    mv a1, x0
    mv a2, x0
    mv a3, x0
    mv a4, x0
    mv a5, x0
    mv a6, x0
    mv a7, x0
    mv s0, x0
    mv s1, x0
    mv s2, x0
    mv s3, x0
    mv s4, x0
    mv s5, x0
    mv s6, x0
    mv s7, x0
    mv s8, x0
    mv s9, x0
    mv s10, x0
    mv s11, x0

    // Clear FP registers (device has F and D extensions)
    fcvt.d.w f0, zero
    fcvt.d.w f1, zero
    fcvt.d.w f2, zero
    fcvt.d.w f3, zero
    fcvt.d.w f4, zero
    fcvt.d.w f5, zero
    fcvt.d.w f6, zero
    fcvt.d.w f7, zero
    fcvt.d.w f8, zero
    fcvt.d.w f9, zero
    fcvt.d.w f10, zero
    fcvt.d.w f11, zero
    fcvt.d.w f12, zero
    fcvt.d.w f13, zero
    fcvt.d.w f14, zero
    fcvt.d.w f15, zero
    fcvt.d.w f16, zero
    fcvt.d.w f17, zero
    fcvt.d.w f18, zero
    fcvt.d.w f19, zero
    fcvt.d.w f20, zero
    fcvt.d.w f21, zero
    fcvt.d.w f22, zero
    fcvt.d.w f23, zero
    fcvt.d.w f24, zero
    fcvt.d.w f25, zero
    fcvt.d.w f26, zero
    fcvt.d.w f27, zero
    fcvt.d.w f28, zero
    fcvt.d.w f29, zero
    fcvt.d.w f30, zero
    fcvt.d.w f31, zero

    // Park core — the host wakes cores via hardware interrupt, which dispatches
    // to the handler address stored in CHIMERA_SNITCH_INTR_HANDLER_ADDR_REG.
__park:
    wfi
    j __park
