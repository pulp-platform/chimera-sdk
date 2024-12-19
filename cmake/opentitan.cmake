# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>

include(ExternalProject)

set(OPENTITAN_DIR ${CMAKE_CURRENT_BINARY_DIR}/../drivers/opentitan)
set(SPARSE_CHECKOUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/sparse-checkout)

set(SPARSE_PATTERNS
  "util/design/*"
  "util/reggen/*"
  "util/regtool.py"
  "util/version_file.py"
  "hw/ip/uart/data/uart.hjson"
  "sw/device/lib/base/abs_mmio.c"
  "sw/device/lib/base/abs_mmio.h"
  "sw/device/lib/base/bitfield.c"
  "sw/device/lib/base/bitfield.h"
  "sw/device/lib/base/macros.h"
  "sw/device/lib/base/math.c"
  "sw/device/lib/base/math.h"
  "sw/device/lib/base/math_builtins.c"
  "sw/device/lib/base/memory.c"
  "sw/device/lib/base/memory.h"
)

file(WRITE ${SPARSE_CHECKOUT_FILE} "")
foreach(PATTERN ${SPARSE_PATTERNS})
  file(APPEND ${SPARSE_CHECKOUT_FILE} "${PATTERN}\n")
endforeach()

# Clone and set up the OpenTitan repository during configuration
if(NOT EXISTS ${OPENTITAN_DIR}/.git)
  message(STATUS "Cloning OpenTitan repository...")
  execute_process(
    COMMAND git clone --no-checkout https://github.com/lowRISC/opentitan.git ${OPENTITAN_DIR}
    RESULT_VARIABLE GIT_CLONE_RESULT
    OUTPUT_VARIABLE GIT_CLONE_OUTPUT
    ERROR_VARIABLE GIT_CLONE_ERROR
  )
  if(NOT GIT_CLONE_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to clone OpenTitan repository: ${GIT_CLONE_ERROR}")
  endif()
else()
  message(STATUS "OpenTitan repository already exists.")
endif()

# Configure sparse checkout
message(STATUS "Configuring sparse checkout for OpenTitan...")
execute_process(
  COMMAND git -C ${OPENTITAN_DIR} config core.sparseCheckout true
  RESULT_VARIABLE GIT_CONFIG_RESULT
  OUTPUT_VARIABLE GIT_CONFIG_OUTPUT
  ERROR_VARIABLE GIT_CONFIG_ERROR
)
if(NOT GIT_CONFIG_RESULT EQUAL 0)
  message(FATAL_ERROR "Failed to configure sparse checkout: ${GIT_CONFIG_ERROR}")
endif()

file(COPY ${SPARSE_CHECKOUT_FILE} DESTINATION ${OPENTITAN_DIR}/.git/info/)

# Apply sparse checkout
execute_process(
  COMMAND git -C ${OPENTITAN_DIR} read-tree -mu HEAD
  RESULT_VARIABLE GIT_READ_TREE_RESULT
  OUTPUT_VARIABLE GIT_READ_TREE_OUTPUT
  ERROR_VARIABLE GIT_READ_TREE_ERROR
)
if(NOT GIT_READ_TREE_RESULT EQUAL 0)
  message(FATAL_ERROR "Failed to apply sparse checkout: ${GIT_READ_TREE_ERROR}")
endif()

# Checkout the desired branch (e.g., master)
execute_process(
  COMMAND git -C ${OPENTITAN_DIR} checkout master
  RESULT_VARIABLE GIT_CHECKOUT_RESULT
  OUTPUT_VARIABLE GIT_CHECKOUT_OUTPUT
  ERROR_VARIABLE GIT_CHECKOUT_ERROR
)
if(NOT GIT_CHECKOUT_RESULT EQUAL 0)
  message(FATAL_ERROR "Failed to checkout branch 'master': ${GIT_CHECKOUT_ERROR}")
endif()

message(STATUS "OpenTitan repository prepared.")

# Now proceed with adding the external project to manage build steps
ExternalProject_Add(
  opentitan_build
  SOURCE_DIR ${OPENTITAN_DIR}
  CONFIGURE_COMMAND ""    # No configure step needed
  BUILD_COMMAND ""        # No build step needed (unless you have specific build steps)
  INSTALL_COMMAND ""      # No install step needed
  BUILD_BYPRODUCTS ${OPENTITAN_DIR}/.git/config
)

# Define directories for OpenTitan sources
set(OPENTITAN_SW_DIR ${OPENTITAN_DIR}/sw)
set(OPENTITAN_HW_IP_DIR ${OPENTITAN_DIR}/hw/ip)
set(OPENTITAN_UTIL_DIR ${OPENTITAN_DIR}/util)

# Paths for regtool and UART headers
set(REGTOOL_PY ${OPENTITAN_UTIL_DIR}/regtool.py)
set(UART_HJSON ${OPENTITAN_HW_IP_DIR}/uart/data/uart.hjson)
set(UART_REGS_H ${OPENTITAN_SW_DIR}/device/lib/dif/autogen/uart_regs.h)

# Define the output directory
get_filename_component(UART_REGS_DIR ${UART_REGS_H} DIRECTORY)

# Custom command to generate UART registers header
add_custom_command(
  OUTPUT ${UART_REGS_H}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${UART_REGS_DIR}
  COMMAND ${Python3_EXECUTABLE} ${REGTOOL_PY} ${UART_HJSON} -D > ${UART_REGS_H}
  DEPENDS ${REGTOOL_PY} ${UART_HJSON}
  COMMENT "Generating ${UART_REGS_H} with COMMAND ${Python3_EXECUTABLE} ${REGTOOL_PY} ${UART_HJSON} -D > ${UART_REGS_H}"
)

add_custom_target(
  generate_uart_regs ALL
  DEPENDS ${UART_REGS_H}
)

add_dependencies(generate_uart_regs opentitan_build)

file(GLOB_RECURSE OPENTITAN_SOURCES
  LIST_DIRECTORIES false
  ${OPENTITAN_SW_DIR}/device/lib/base/*.c
)

message(STATUS "OpenTitan sources collected.")

# Create OpenTitan library
add_library(opentitan_lib STATIC ${OPENTITAN_SOURCES})

# Ensure the library depends on the external project build
add_dependencies(opentitan_lib opentitan_build generate_uart_regs)

# Include directories for OpenTitan library
target_include_directories(opentitan_lib
    PUBLIC
    ${OPENTITAN_DIR}
)

target_link_libraries(opentitan_lib
    PUBLIC
    runtime_host
)

message(STATUS "OpenTitan library configured.")

