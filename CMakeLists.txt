# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.22)

# Check wehter TOOLCHAIN_DIR and PICOLIBC_DIR are correctly defined
if (NOT DEFINED TOOLCHAIN_DIR)
    message(FATAL_ERROR "TOOLCHAIN_DIR is not defined. Please set it to the path of the RISC-V toolchain.")
endif()

if (NOT DEFINED PICOLIBC_DIR)
    message(FATAL_ERROR "PICOLIBC_DIR is not defined. Please set it to the path of the Picolibc library.")
endif()

# VIVIANEP: We need the C11 standard (or later) for building
#           the OpenTitan peripheral drivers
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# SCHEREMO: Needed to skip compiler test, which doesn't support baremetal targets
set(CMAKE_C_COMPILER_WORKS 1)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
# include(cmake/debug.cmake)

set(DISASSEMBLE_LIBRARIES CACHE BOOL OFF)

set(CMAKE_TOOLCHAIN_FILE cmake/toolchain_llvm.cmake)

include("targets/${TARGET_PLATFORM}/config.cmake")

# extract base ISA (e.g. rv32ima from rv32ima_xdma)
string(REGEX MATCH "^[a-z0-9]+" ISA_HOST_BASE ${ISA_HOST})
string(REGEX MATCH "^[a-z0-9]+" ISA_CLUSTER_SNITCH_BASE ${ISA_CLUSTER_SNITCH})

project(chimera-sdk LANGUAGES C ASM)

if(${DISASSEMBLE_LIBRARIES})
  message(STATUS "[CHIMERA-SDK] DISASSEMBLE_LIBRARIES : ON")
else()
  message(STATUS "[CHIMERA-SDK] DISASSEMBLE_LIBRARIES : OFF")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmake/Utils.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/Chimera.cmake)

# Add CMAKE_BINARY_DIR to linker search path so *_placement.ldh files are found
add_link_options(-L${CMAKE_BINARY_DIR})

option(CHIMERA_UNIFIED_ELF "Merge all ELFs into one unified mixed-ISA ELF" ON)

set(error_messages "")

################################################################################
# Python Tools                                                                #
################################################################################

# Bootstrap uv Python environment for post-build tools (optional)
find_program(CHIMERA_UV uv
    HINTS $ENV{HOME}/.local/bin /usr/local/bin
    DOC "uv Python package manager")
if(CHIMERA_UV)
    execute_process(
        COMMAND "${CHIMERA_UV}" sync
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        RESULT_VARIABLE _uv_sync_result
        OUTPUT_QUIET ERROR_QUIET
    )
    if(NOT _uv_sync_result EQUAL 0)
        message(WARNING "[CHIMERA-SDK] uv sync failed (exit ${_uv_sync_result}); overlap checker may not work.")
    endif()
else()
    message(STATUS "[CHIMERA-SDK] 'uv' not found; chimera_check_overlaps will be skipped.")
endif()

################################################################################
# Add subdirectories                                                           #
################################################################################
# WIESEP: Targets have to be included before the other folders to make them available
# Depending on the target, the following static libraries have to added by the targets:
# - runtime_host
# - runtime_cluster_snitch
add_subdirectory(targets)

# Include other subdirectories
add_subdirectory(shared)
add_subdirectory(devices)
add_subdirectory(host)

# Materialize accumulated global runtime properties into regular variables.
# These variables are inherited by all subsequent add_subdirectory() calls (tests).
get_property(CHIMERA_HOST_RUNTIME_SOURCES            GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_SOURCES)
get_property(CHIMERA_HOST_RUNTIME_INCLUDES           GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_INCLUDES)
get_property(CHIMERA_HOST_RUNTIME_DEFINES            GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_DEFINES)
get_property(CHIMERA_HOST_RUNTIME_PRIVATE_INCLUDES   GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_PRIVATE_INCLUDES)
get_property(CHIMERA_DEVICE_RUNTIME_SOURCES          GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_SOURCES)
get_property(CHIMERA_DEVICE_RUNTIME_INCLUDES         GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_INCLUDES)
get_property(CHIMERA_DEVICE_RUNTIME_PRIVATE_INCLUDES GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_PRIVATE_INCLUDES)
get_property(CHIMERA_DEVICE_RUNTIME_DEFINES          GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_DEFINES)

################################################################################
# Testing Tools                                                                #
################################################################################

set(VALID_HARDWARE_BACKEND "RTL" "GVSoC" "ASIC")
set(HARDWARE_BACKEND "GVSoC" CACHE STRING "Toogle the chimera backend")

# JUNGVI: "simulation" compiles and run the test, "compile" just compiles the tests, and none does neither.
set(VALID_TEST_MODES "simulation" "compile" "none")

# Config Variables Default Values
set(TEST_MODE "compile" CACHE STRING "Set what validation target the tests should be run on")

if(NOT HARDWARE_BACKEND IN_LIST VALID_HARDWARE_BACKEND)
  list(APPEND error_messages "[CHIMERA-SDK] Invalid HARDWARE_BACKEND: ${HARDWARE_BACKEND}")
  list(APPEND error_messages "[CHIMERA-SDK] Available options for HARDWARE_BACKEND are:")
  foreach(mode IN LISTS VALID_HARDWARE_BACKEND)
    list(APPEND error_messages " - ${mode}")
  endforeach()
else()
  message(STATUS "[CHIMERA-SDK] Hardware Backend      : ${HARDWARE_BACKEND}")
endif()

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
set(SIMULATION_BINARY ${SOC_MODEL_BINARY})
set(PRELOAD_MODE "0" CACHE STRING "Preload mode for simulation")

if(TEST_MODE STREQUAL "compile" OR TEST_MODE STREQUAL "simulation")
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
