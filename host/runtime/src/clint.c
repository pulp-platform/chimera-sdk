// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifdef CHIMERA_DRIVER_CLINT

// Include Standard Libraries
#include <stdio.h>
#include <stddef.h>

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers

// Include Runtime Headers
#include "clint.h"
#include "util.h"

chi_interrupt_t default_clint_inst = {
    .api = &default_clint_api,
    .base = (uintptr_t)&__base_clint,
    .cfg = NULL,
};

// Prevent function removal by linker
void default_trap_vector(void) {
    // Check if it was a MSIP interrupt
    uint32_t mip = get_mip();
    if (mip & (1 << 3)) { // Check if it was a MSIP interrupt
        // Clear the MSIP interrupt
        *reg32(&__base_clint, CLINT_MSIP_REG_OFFSET) = 0;
        return;
    } else if (mip & (1 << 7)) { // Check if it was a MTIP interrupt
        // Clear MTIP interrupt by disabling it temporarily
        set_mtie(0);
    }
}

#endif // CHIMERA_DRIVER_CLINT