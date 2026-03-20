// SPDX-FileCopyrightText: 2023 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#pragma once

/**
 * @defgroup device_snitchCluster Snitch Cluster Device Runtime
 * @brief Runtime library for code executing on Snitch cluster cores.
 *
 * Provides initialisation, I/O, and synchronisation primitives for
 * RV32 device-side code running on a Snitch cluster.
 * @{
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// Initialization functions
void snrt_init();
void snrt_exit(int exit_code);
void snrt_printf_init();

// Standard I/O streams for Snitch cluster
extern FILE *const stdin;
extern FILE *const stdout;
extern FILE *const stderr;

// Retarget printf to stdio of snitch cluster
int snrt_printf_log(const char *fmt, ...);
int snrt_printf(const char *fmt, ...);

#define printf snrt_printf_log

#include "config.h"
#include "util.h"

// Forward declarations
#include "alloc_decls.h"
#include "cls_decls.h"
#include "riscv_decls.h"
#include "start_decls.h"
#include "sync_decls.h"
#include "team_decls.h"

// Snitch cluster specific
#include "snitch_cluster_defs.h"
#include "snitch_cluster_memory.h"

// Implementation
#include "alloc.h"
#include "alloc_v2.h"
#include "cls.h"
#include "cluster_interrupts.h"
#include "dm.h"
#include "dma.h"
#include "dump.h"
#include "eu.h"
#include "kmp.h"
#include "omp.h"
#include "perf_cnt.h"
#include "riscv.h"
#include "snitch_cluster_global_interrupts.h"
#include "ssr.h"
#include "sync.h"
#include "team.h"
#include "types.h"
#include "start.h"

// Utility macros
/**
 * Places a zero-initialized declaration into the L1 memory of each cluster.
 * Every cluster receives its own copy.
 */
#if defined(__clang__)
#define SNRT_CLUSTER_L1_ZERO(decl) \
    _Pragma("clang section bss = \".cbss\"") decl; \
    _Pragma("clang section bss = \"\"")
#else
#define SNRT_CLUSTER_L1_ZERO(decl) decl __attribute__((section(".cbss,\"aw\",@nobits#")))
#endif

/**
 * Places an initialized declaration into the L1 memory of each cluster.
 * Each cluster receives its own copy.
 */
#define SNRT_CLUSTER_L1_COPY(decl) decl __attribute__((section(".cdata")))

/**
 * Places a initialized declaration into the L1 memory of a specific cluster.
 */
#define SNRT_CLUSTER_L1(decl, cluster_id) decl __attribute__((section(".l1_c" #cluster_id)))

/** @} */
