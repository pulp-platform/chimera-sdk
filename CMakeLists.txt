# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>
# Philip Wiese <wiesep@iis.ee.ethz.ch>

cmake_minimum_required(VERSION 3.24)

# VIVIANEP: We need the C11 standard (or later) for building
#           the OpenTitan peripheral drivers
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# SCHEREMO: Needed to skip compiler test, which doesn't support baremetal targets
set(CMAKE_C_COMPILER_WORKS 1)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
# include(cmake/debug.cmake)

# Set host-specific flags
set(HOST_ARCH "riscv32")
set(HOST_FAMILY "riscv32")
set(HOST_ENDIAN "little")
set(HOST_SYSTEM "none")

# SCHEREMO: This toolchain file is only used for test compilation!
set(CMAKE_TOOLCHAIN_FILE cmake/toolchain_llvm.cmake)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include("targets/${TARGET_PLATFORM}/config.cmake")

# extract base ISA (e.g. rv32ima from rv32ima_xdma)
string(REGEX MATCH "^[a-z0-9]+" ISA_HOST_BASE ${ISA_HOST})
string(REGEX MATCH "^[a-z0-9]+" ISA_CLUSTER_SNITCH_BASE ${ISA_CLUSTER_SNITCH})

project(chimera-sdk LANGUAGES C ASM)

message(STATUS "[CHIMERA-SDK] ABI_HOST              : ${ABI_HOST}")
message(STATUS "[CHIMERA-SDK] ABI_CLUSTER_SNITCH    : ${ABI_CLUSTER_SNITCH}")
message(STATUS "[CHIMERA-SDK] ISA_HOST              : ${ISA_HOST}")
message(STATUS "[CHIMERA-SDK] ISA_CLUSTER_SNITCH    : ${ISA_CLUSTER_SNITCH}")
message(STATUS "[CHIMERA-SDK] Picolibc Host         : ${PICOLIB_HOST}")
message(STATUS "[CHIMERA-SDK] Picolibc Snitch       : ${PICOLIB_CLUSTER_SNITCH}")
message(STATUS "[CHIMERA-SDK] Compiler RT Host      : ${COMPILERRT_HOST}")
message(STATUS "[CHIMERA-SDK] Compiler RT Snitch    : ${COMPILERRT_CLUSTER_SNITCH}")

if(${DISASSEMBLE_LIBRARIES})
  message(STATUS "[CHIMERA-SDK] DISASSEMBLE_LIBRARIES : ON")
else()
  message(STATUS "[CHIMERA-SDK] DISASSEMBLE_LIBRARIES : OFF")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmake/Utils.cmake)

set(VALID_HARDWARE_BACKEND "RTL" "GVSoC" "ASIC")
set(HARDWARE_BACKEND "GVSoC" CACHE STRING "Toogle the chimera backend")

set(error_messages "")


################################################################################
# Add subdirectories                                                           #
################################################################################
# WIESEP: Targets have to be included before the other folders to make them available
# Depending on the target, the following static libraries have to added by the targets:
# - runtime_host
# - runtime_cluster_snitch
add_subdirectory(targets)

# VIVIANEP: Include the picolibc library
include(${CMAKE_CURRENT_LIST_DIR}/cmake/picolibc.cmake)

# Include other subdirectories
add_subdirectory(devices)
add_subdirectory(host)

################################################################################
# Testing                                                                      #
################################################################################

# JUNGVI: "simulation" compiles and run the test, "compile" just compiles the tests, and none does neither.
set(VALID_TEST_MODES "simulation" "compile" "none")

if(NOT HARDWARE_BACKEND IN_LIST VALID_HARDWARE_BACKEND)
  list(APPEND error_messages "[CHIMERA-SDK] Invalid HARDWARE_BACKEND: ${HARDWARE_BACKEND}")
  list(APPEND error_messages "[CHIMERA-SDK] Available options for HARDWARE_BACKEND are:")
  foreach(mode IN LISTS VALID_HARDWARE_BACKEND)
    list(APPEND error_messages " - ${mode}")
  endforeach()
else()
  message(STATUS "[CHIMERA-SDK] Hardware Backend      : ${HARDWARE_BACKEND}")
endif()

# Config Variables Default Values
set(TEST_MODE "compile" CACHE STRING "Set what validation target the tests should be run on")

if(NOT TEST_MODE IN_LIST VALID_TEST_MODES)
  list(APPEND error_messages "[CHIMERA-SDK] Invalid TEST_MODE: ${TEST_MODE}")
  list(APPEND error_messages "[CHIMERA-SDK] Available options for TEST_MODE are:")
  foreach(mode IN LISTS VALID_TEST_MODES)
    list(APPEND error_messages " - ${mode}")
  endforeach()
endif()

if(error_messages)
  list(JOIN error_messages "\n" error_message)
  message(FATAL_ERROR "[CHIMERA-SDK] ${error_message}")
endif()

set(SOC_MODEL_BINARY "chim.bin" CACHE STRING "Path to the simulation binary")
set(PRELOAD_MODE "0" CACHE STRING "Preload mode for simulation")

if(TEST_MODE STREQUAL "compile")
  add_subdirectory(tests)
endif()

if(TEST_MODE STREQUAL "simulation")
  enable_testing()

  if(NOT EXISTS ${SOC_MODEL_BINARY})
    message(FATAL_ERROR "[CHIMERA-SDK] SoC model binary ${SOC_MODEL_BINARY} does not exist.")
  endif()

  # Convert PRELOAD_MODE to integer
  if("${PRELOAD_MODE}" MATCHES "^[0-9]+$")
    set(PRELOAD_MODE_INT ${PRELOAD_MODE})
  else()
    message(FATAL_ERROR "PRELOAD_MODE must be an integer.")
  endif()

  message(STATUS "[CHIMERA-SDK] SoC Model Binary      : ${SOC_MODEL_BINARY}")
  message(STATUS "[CHIMERA-SDK] Preload Mode          : ${PRELOAD_MODE}")
endif()
