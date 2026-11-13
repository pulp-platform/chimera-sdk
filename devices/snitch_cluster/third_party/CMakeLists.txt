# Copyright 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set(SNITCH_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/snitch-sdk)
set(ITA_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ita-sdk)
set(SN_DEPS_DIR "${SNITCH_ROOT_DIR}/deps" CACHE STRING "Directory for external dependencies")


# -----------------------------------------------------------------------------
# Add Subdirectories for Runtime Components
# -----------------------------------------------------------------------------
set(C_SOURCES
    "${SNITCH_ROOT_DIR}/core/src/alloc.c"
    "${SNITCH_ROOT_DIR}/core/src/alloc_v2.c"
    "${SNITCH_ROOT_DIR}/core/src/cls.c"
    "${SNITCH_ROOT_DIR}/core/src/cluster_interrupts.c"
    "${SNITCH_ROOT_DIR}/core/src/dm.c"
    "${SNITCH_ROOT_DIR}/core/src/dma.c"
    "${SNITCH_ROOT_DIR}/core/src/global_interrupts.c"
    "${SNITCH_ROOT_DIR}/core/src/perf_cnt.c"
    # WIESEP: We use the picolibc implementation of printf
    # "${SNITCH_ROOT_DIR}/printf.c"
    # "${SNITCH_ROOT_DIR}/snrt_putchar.c"
    "${SNITCH_ROOT_DIR}/core/src/riscv.c"
    # WIESEP: Start files are included in this runtime library
    # "${SNITCH_ROOT_DIR}/start.S"
    # "${SNITCH_ROOT_DIR}/start.c"
    "${SNITCH_ROOT_DIR}/core/src/ssr.c"
    "${SNITCH_ROOT_DIR}/core/src/sync.c"
    "${SNITCH_ROOT_DIR}/core/src/team.c"
    "${SNITCH_ROOT_DIR}/omp/src/eu.c"
    "${SNITCH_ROOT_DIR}/omp/src/kmp.c"
    "${SNITCH_ROOT_DIR}/omp/src/omp.c"
    "${SNITCH_ROOT_DIR}/target/common/src/snitch_cluster_memory.c"
)

chimera_add_device_runtime(snitch_cluster
    # These hold the out-of-line copies of the runtime's inline functions, most of them unused
    LIBRARY_SOURCES ${C_SOURCES}
    SOURCES         "${ITA_ROOT_DIR}/src/ITA_utils.c"
    INCLUDES
        ${SN_DEPS_DIR}
        ${SNITCH_ROOT_DIR}/target/common/inc
        ${SNITCH_ROOT_DIR}/core/inc
        ${SNITCH_ROOT_DIR}/omp/inc
        ${ITA_ROOT_DIR}/inc
        ${SNITCH_ROOT_DIR}/deps
        ${SNITCH_ROOT_DIR}/deps/idma
    PRIVATE_INCLUDES
        ${SNITCH_ROOT_DIR}/core/src
        ${SNITCH_ROOT_DIR}/omp/src
)
