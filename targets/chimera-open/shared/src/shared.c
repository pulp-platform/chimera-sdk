// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#include "shared.h"

/*
 * Shared data instance placed in the '.common' section at a fixed address.
 *
 * Host linker script:  .common is a LOAD section at 0x48000000.
 * Device linker script: .common is NOLOAD at the same address — the device
 * accesses the host-initialized memory via the AXI interconnect.
 *
 * aligned(64) ensures the struct starts on a 64-byte boundary as required by
 * the HTIF semihosting protocol (host_to_device / device_to_host must each be
 * reachable at a naturally-aligned address within the struct).
 */
chimera_shared_data_t shared_data __attribute__((section(".common"), aligned(64)));
