# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna.
# SPDX-License-Identifier: Apache-2.0

################################################################################
# Host Runtime Library                                                         #
################################################################################
add_library(runtime_host STATIC)

target_link_directories(runtime_host PUBLIC
  ${TOOLCHAIN_DIR}/lib/clang/${LLVM_VERSION}/lib/baremetal/${COMPILERRT_HOST}
)

# WIESEP: Make sure picolibc exists before building the host runtime for the header files
add_dependencies(runtime_host picolibc)

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
  ${CMAKE_CURRENT_LIST_DIR}/inc
  ${CMAKE_CURRENT_LIST_DIR}/inc/regs
  ${PROJECT_SOURCE_DIR}/host/hal/inc
)

target_compile_options(runtime_host
  PRIVATE
  -O2
)

target_compile_definitions(runtime_host PUBLIC TARGET_PLATFORM_CHIMERA_HOST)

target_compile_options(runtime_host
  PUBLIC
  --target=riscv32-unknown-elf
  -march=${ISA_HOST}
  -mabi=${ABI_HOST}
)

# WIESEP: Export the target specific linkerscript
target_link_options(runtime_host
  PUBLIC
  -L${CMAKE_CURRENT_LIST_DIR}
  -Tlink.ld
  --target=riscv32-unknown-elf
  -mabi=${ABI_HOST}
  -nostartfiles
  -ffreestanding
)

################################################################################
# Snitch Cluster Runtime Library                                               #
################################################################################
# WIESEP: There is no Snitch cluster present in this target