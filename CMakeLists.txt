# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>
# Philip Wiese <wiesep@iis.ee.ethz.ch>

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 99)

# SCHEREMO: Needed to skip compiler test, which doesn't support baremetal targets
set(CMAKE_C_COMPILER_WORKS 1)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

# SCHEREMO: Help most IDE's LSPs find definitions
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# SCHEREMO: This toolchain file is only used for test compilation!
set(CMAKE_TOOLCHAIN_FILE cmake/toolchain_llvm.cmake)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(chimera-sdk LANGUAGES C ASM)

# WIESEP: It is important to set the ISA and ABI for the host and the cluster snitch
set(ISA_HOST rv32imc)
set(ABI_HOST ilp32)
set(ISA_CLUSTER_SNITCH rv32im)
set(ABI_CLUSTER_SNITCH ilp32)

message(STATUS "[CHIMERA-SDK] ISA_HOST           : ${ISA_HOST}")
message(STATUS "[CHIMERA-SDK] ABI_HOST           : ${ABI_HOST}")
message(STATUS "[CHIMERA-SDK] ISA_CLUSTER_SNITCH : ${ISA_CLUSTER_SNITCH}")
message(STATUS "[CHIMERA-SDK] ABI_CLUSTER_SNITCH : ${ABI_CLUSTER_SNITCH}")

include(${CMAKE_CURRENT_LIST_DIR}/cmake/Utils.cmake)

# WIESEP: Add a object library to collect all runtime sources
add_library(runtime STATIC)

target_compile_options(runtime
  PRIVATE
  -O2
  -march=${ISA_HOST}
  -mabi=${ABI_HOST}
)

target_link_options(runtime
  PRIVATE
  -march=${ISA_HOST}
  -mabi=${ABI_HOST}
)

# WIESEP: Expose common link option
target_link_options(runtime
  PUBLIC
  -nostartfiles
  -nostdlib
)

add_subdirectory(hal)
add_subdirectory(targets)
add_subdirectory(driver)

# WIESEP: Interface library to link against all components of the SDK
add_library(chimera-sdk INTERFACE)
target_link_libraries(chimera-sdk INTERFACE hal)
target_link_libraries(chimera-sdk INTERFACE runtime)

enable_testing()

add_subdirectory(tests)
