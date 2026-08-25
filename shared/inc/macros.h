// SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/**
 * @defgroup shared_macros Shared Utility Macros
 * @brief Cross-cutting compiler and linker utility macros for Chimera-SDK.
 * @{
 */

#pragma once

/**
 * @brief Prevents the compiler and linker from discarding a symbol.
 *
 * Combines @c __attribute__((used)) (tells the compiler to emit the symbol
 * even if it appears unreferenced) with @c __attribute__((retain)) on
 * toolchains that support it (prevents the linker from garbage-collecting
 * the section).  Falls back to @c used alone on older toolchains.
 *
 * Use on symbols that are referenced only from assembly (e.g. trampolines,
 * interrupt vectors) or from linker scripts.
 */
#if defined(__has_attribute)
#if __has_attribute(retain)
#define KEEP __attribute__((used, retain))
#else
#define KEEP __attribute__((used))
#endif
#else
#define KEEP __attribute__((used))
#endif

/** @} */