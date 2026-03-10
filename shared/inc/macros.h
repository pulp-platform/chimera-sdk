// SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#pragma once

#if defined(__has_attribute)
#if __has_attribute(retain)
#define KEEP __attribute__((used, retain))
#else
#define KEEP __attribute__((used))
#endif
#else
#define KEEP __attribute__((used))
#endif