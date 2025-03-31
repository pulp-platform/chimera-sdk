// Copyright 2022 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Authors: Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#include <stdlib.h>

__attribute__((weak, warning("64-bit arithmetic operation detected; ensure you're using the "
                             "intended alternative implementation."))) void
_ot_builtin_div64_intentionally_not_implemented_see_pull_11451(void) {
    // Abort execution if this function is ever actually called.
    abort();
}