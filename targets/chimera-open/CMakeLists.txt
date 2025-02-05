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

set(TARGET "chimera-open" CACHE STRING "Target platform")

set(CROSS_C_COMPILER "clang" CACHE STRING "Cross compiler")
set(CROSS_AR "llvm-ar" CACHE STRING "Archiver")
set(CROSS_STRIP "llvm-strip" CACHE STRING "Strip tool")
set(TARGET_FLAG "riscv32-unknown-elf" CACHE STRING "Target flag")

set(CROSS_CPU "riscv32" CACHE STRING "CPU Architecture")
set(CROSS_CPU_FAMILY "riscv32" CACHE STRING "CPU Family")
set(CROSS_ENDIAN "little" CACHE STRING "Endianness")
set(CROSS_SYSTEM "none" CACHE STRING "System type")

set(CROSS_C_COMPILER_ARGS "-target ${TARGET_FLAG} -march=${ISA_HOST} -nostdlib" CACHE STRING "Compiler arguments")
set(CROSS_C_ARGS "-Werror=double-promotion -Wno-unsupported-floating-point-opt -fshort-enums -mno-relax -march=${ISA_HOST} -mabi=${ABI}" CACHE STRING "C arguments")
set(CROSS_C_LINK_ARGS "-Wl,-z,noexecstack -march=${ISA_HOST} -mabi=${ABI}" CACHE STRING "Linker arguments")

set(CROSS_SKIP_SANITY_CHECK "true" CACHE STRING "Skip sanity check")

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
add_library(runtime_cluster_snitch STATIC)

# WIESEP: Export the headers also to the cluster runtime
target_include_directories(runtime_cluster_snitch
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
)

# WIESEP: Do not export optimization flags
target_compile_options(runtime_cluster_snitch
  PRIVATE
  -O2
)

target_compile_options(runtime_cluster_snitch
  PUBLIC
  -march=${ISA_CLUSTER_SNITCH}
  -mabi=${ABI}
)

# WIESEP: Expose common link option
target_link_options(runtime_cluster_snitch
  PUBLIC
  -march=${ISA_CLUSTER_SNITCH}
  -mabi=${ABI}
  -nostartfiles
  -ffreestanding
)