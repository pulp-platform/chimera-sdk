# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>
# Philip Wiese <wiesep@iis.ee.ethz.ch>

################################################################################
# Set up picolibc
################################################################################

set(TARGET "chimera-host")

set(CROSS_C_COMPILER "clang")
set(CROSS_AR "llvm-ar")
set(CROSS_STRIP "llvm-strip")
set(TARGET_FLAG "riscv32-unknown-elf")

set(CROSS_CPU "riscv32")
set(CROSS_CPU_FAMILY "riscv32")
set(CROSS_ENDIAN "little")
set(CROSS_SYSTEM "none")

set(CROSS_C_COMPILER_ARGS "-target ${TARGET_FLAG} -march=${ISA_HOST} -nostdlib")
set(CROSS_C_ARGS "-Werror=double-promotion -Wno-unsupported-floating-point-opt -fshort-enums -mno-relax -march=${ISA_HOST} -mabi=${ABI}")
set(CROSS_C_LINK_ARGS "-Wl,-z,noexecstack -march=${ISA_HOST} -mabi=${ABI}")

set(CROSS_SKIP_SANITY_CHECK "true")

include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/picolibc.cmake)

################################################################################
# Host Runtime Library                                                         #
################################################################################

# VIVIANEP: Select target-specific commit hash of the OT lib
set(TARGET_OPENTITAN_COMMIT_HASH 22168a3d42febfca725b36dd0d1554b68cf5783e)
set(OPENTITAN_COMMIT_HASH ${TARGET_OPENTITAN_COMMIT_HASH} CACHE STRING "OpenTitan commit hash")


add_library(runtime_host STATIC)

file(GLOB_RECURSE ASM_SOURCES
  "src/crt0.S"
)

file(GLOB_RECURSE C_SOURCES
  "src/*.c"
)

set_property(SOURCE ${ASM_SOURCES} PROPERTY LANGUAGE ASM)

target_sources(runtime_host
  PRIVATE
  ${ASM_SOURCES}
  ${C_SOURCES}
)

# WIESEP: Export the target specific include directory
target_include_directories(runtime_host
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
)

target_compile_options(runtime_host
  PRIVATE
  -O2
)

target_compile_options(runtime_host
  PUBLIC
  -march=${ISA_HOST}
  -mabi=${ABI}
)

# VIVIANEP: Must set s
target_compile_definitions(runtime_host
  PUBLIC
  -DOT_PLATFORM_RV32=1
)

# WIESEP: Export the target specific linkerscript
target_link_options(runtime_host
  PUBLIC
  -L${CMAKE_CURRENT_LIST_DIR}
  -Tlink.ld
  -march=${ISA_HOST}
  -mabi=${ABI}
  -nostartfiles
  -ffreestanding
)

################################################################################
# Snitch Cluster Runtime Library                                               #
################################################################################
# WIESEP: There is no Snitch cluster present in this target