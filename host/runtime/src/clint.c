// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#ifdef CHIMERA_DRIVER_CLINT

// Include Standard Libraries
#include <stdio.h>
#include <stddef.h>

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers

// Include Runtime Headers
#include "clint.h"

chi_interrupt_t default_clint_inst = {
    .api = &default_clint_api,
    .base = (uintptr_t)&__base_clint,
    .cfg = NULL,
};

#endif // CHIMERA_DRIVER_CLINT