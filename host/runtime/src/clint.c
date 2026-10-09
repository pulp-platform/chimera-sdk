// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/*
 * CLINT runtime singleton and default trap vector.
 *
 * default_clint_inst is the module-level chi_interrupt_t instance; its base
 * address is resolved from the __base_clint linker symbol provided by soc.h.
 *
 * default_trap_vector() is the minimal machine-mode trap handler installed by
 * crt0.S.  It only handles the two CLINT interrupt sources:
 *  - MSIP (bit 3 of mip): software interrupt; cleared by writing 0 to CLINT_MSIP.
 *    Used by the Snitch putc semihosting path to notify the host.
 *  - MTIP (bit 7 of mip): timer interrupt; cleared by disabling mtie so that
 *    clint_sleep_until() can return once the wfi wakes up.
 * All other trap causes (exceptions, external interrupts) are unhandled and will
 * loop or fall through depending on the calling context.
 */

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