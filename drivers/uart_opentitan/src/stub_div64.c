// Copyright 2022 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Authors: Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#include <stdlib.h>

/*
 * This stub satisfies OpenTitan’s trap-only builtins (in math_builtins.c)
 * which call this symbol when they catch a 64-bit divide or modulo:
 *
 *   __divdi3, __udivdi3, __moddi3, __umoddi3
 *
 * On a 32-bit host build (e.g. with picolibc), many library routines
 * (time functions, printf, strto*, etc.) legitimately use 64-bit
 * arithmetic and emit calls to the Clang runtime helpers.  We link
 * in the real compiler-rt builtins (clang_rt.builtins-riscv32.a) to
 * supply proper software implementations of those helpers—but we
 * still need to provide this symbol so that the weak OpenTitan traps
 * resolve at link time without error.
 *
 * We leave the warning attribute in place so that if any code
 * accidentally *calls* this stub at runtime (e.g. device-side),
 * it will abort and produce a clear diagnostic about 64-bit math
 * usage on an unsupported platform.
 */
__attribute__((weak, warning("64-bit arithmetic operation detected; ensure you're using the "
                             "intended alternative implementation."))) void
_ot_builtin_div64_intentionally_not_implemented_see_pull_11451(void) {
    // Host-side: no-op (should never be hit in normal host tests)
    abort();
}
