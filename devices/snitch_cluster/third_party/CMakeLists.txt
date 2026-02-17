# Copyright 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set(SNITCH_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/snitch-sdk)
set(SN_DEPS_DIR "${SNITCH_ROOT_DIR}/deps" CACHE STRING "Directory for external dependencies")


# -----------------------------------------------------------------------------
# Add Subdirectories for Runtime Components
# -----------------------------------------------------------------------------
file(GLOB_RECURSE C_SOURCES
    "${SNITCH_ROOT_DIR}/alloc.c"
    "${SNITCH_ROOT_DIR}/alloc_v2.c"
    "${SNITCH_ROOT_DIR}/cls.c"
    "${SNITCH_ROOT_DIR}/dm.c"
    "${SNITCH_ROOT_DIR}/global_interrupts.c"
    "${SNITCH_ROOT_DIR}/perf_cnt.c"
    "${SNITCH_ROOT_DIR}/riscv.c"
    # WIESEP: We use the picolibc implementation of printf
    # "${SNITCH_ROOT_DIR}/printf.c"
    # "${SNITCH_ROOT_DIR}/snrt_putchar.c"
    # WIESEP: Start files are included in this runtime library
    # "${SNITCH_ROOT_DIR}/start.S"
    # "${SNITCH_ROOT_DIR}/start.c"
    "${SNITCH_ROOT_DIR}/sync.c"
    "${SNITCH_ROOT_DIR}/omp/src/eu.c"
    "${SNITCH_ROOT_DIR}/omp/src/kmp.c"
    "${SNITCH_ROOT_DIR}/omp/src/omp.c"
)


target_sources(runtime_cluster_snitch PRIVATE ${C_SOURCES})

# Remove -Wundefined-inline warnings from snitch sources (only for these files)
set_source_files_properties(
    ${C_SOURCES}
    PROPERTIES APPEND_STRING PROPERTY COMPILE_FLAGS "-Wno-undefined-inline"
)

target_include_directories(runtime_cluster_snitch
    PUBLIC
    ${SN_DEPS_DIR}
    ${SNITCH_ROOT_DIR}/target/common/inc
    ${SNITCH_ROOT_DIR}/core/inc
    ${SNITCH_ROOT_DIR}/omp/inc
)

target_include_directories(runtime_cluster_snitch
    PRIVATE
    ${SNITCH_ROOT_DIR}/core/src
    ${SNITCH_ROOT_DIR}/omp/src
)
