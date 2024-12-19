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

# SCHEREMO: This toolchain file is only used for test compilation!
set(CMAKE_TOOLCHAIN_FILE cmake/toolchain_llvm.cmake)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(chimera-sdk LANGUAGES C ASM)

# WIESEP: It is important to set the ISA and ABI for the host and the cluster snitch
set(ABI ilp32)
set(ISA_CLUSTER_SNITCH rv32im)
set(ISA_HOST rv32imc)

message(STATUS "[CHIMERA-SDK] ABI                   : ${ABI}")
message(STATUS "[CHIMERA-SDK] ISA_HOST              : ${ISA_HOST}")
message(STATUS "[CHIMERA-SDK] ISA_CLUSTER_SNITCH    : ${ISA_CLUSTER_SNITCH}")
if (${DISASSEMBLE_LIBRARIES})
  message(STATUS "[CHIMERA-SDK] DISASSEMBLE_LIBRARIES : ON")
else()
  message(STATUS "[CHIMERA-SDK] DISASSEMBLE_LIBRARIES : OFF")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmake/Utils.cmake)

################################################################################
# Add subdirectories                                                           #
################################################################################
# WIESEP: Targets have to be included before the other folders to make them available
# Depending on the target, the following static libraries have to added by the targets:
# - runtime_host
# - runtime_cluster_snitch
add_subdirectory(targets)

# Include other subdirectories
add_subdirectory(hal)
add_subdirectory(devices)
add_subdirectory(drivers)

################################################################################
# Testing                                                                      #
################################################################################

set(VALID_TEST_MODES "simulation" "none")

# Option for TEST_MODE with default value
set(TEST_MODE "none" CACHE STRING "Set what validation target the tests should be run on")

# Check if TEST_MODE is set to a valid option
if(NOT TEST_MODE IN_LIST VALID_TEST_MODES)
  # Collect all error messages into a single string
  set(error_messages "")
  list(APPEND error_messages "[CHIMERA-SDK] Invalid TEST_MODE: ${TEST_MODE}")
  list(APPEND error_messages "[CHIMERA-SDK] Available options for TEST_MODE are:")
  foreach(mode IN LISTS VALID_TEST_MODES)
    list(APPEND error_messages " - ${mode}")
  endforeach()

  # Convert the list of messages into a single multi-line string
  list(JOIN error_messages "\n" error_message)

  # Raise fatal error with all messages
  message(FATAL_ERROR "${error_message}")
endif()

set(SOC_MODEL_BINARY "chim.bin" CACHE STRING "Path to the simulation binary")
set(PRELOAD_MODE "0" CACHE STRING "Preload mode for simulation")

if (TEST_MODE STREQUAL "simulation")
  enable_testing()

  if(NOT EXISTS ${SOC_MODEL_BINARY})
    message(FATAL_ERROR "SoC model binary ${SOC_MODEL_BINARY} does not exist.")
  endif()

  # Convert PRELOAD_MODE to integer
  if ("${PRELOAD_MODE}" MATCHES "^[0-9]+$")
    set(PRELOAD_MODE_INT ${PRELOAD_MODE})
  else()
    message(FATAL_ERROR "PRELOAD_MODE must be an integer.")
  endif()

  message(STATUS "[CHIMERA-SDK] SoC Model Binary      : ${SOC_MODEL_BINARY}")
  message(STATUS "[CHIMERA-SDK] Preload Mode          : ${PRELOAD_MODE}")
endif()

add_subdirectory(tests)
