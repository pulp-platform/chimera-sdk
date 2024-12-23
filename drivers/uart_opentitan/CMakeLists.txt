# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>

# ------------------------------------------------------------------------------
# Set the variables for the OpenTitan UART driver
# ------------------------------------------------------------------------------
set(OPENTITAN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/opentitan")
set(OPENTITAN_SPARSE_PATTERNS
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
  "sw/device/lib/base/mmio.c"
  "sw/device/lib/base/mmio.h"
  "sw/device/lib/base/multibits.c"
  "sw/device/lib/base/multibits.h"
  "sw/device/lib/base/internal/status.h"
  "sw/device/lib/base/internal/absl_status.h"
  "sw/device/lib/dif/dif_base.c"
  "sw/device/lib/dif/dif_base.h"
  "sw/device/lib/dif/dif_uart.c"
  "sw/device/lib/dif/dif_uart.h"
  "sw/device/lib/dif/autogen/dif_uart_autogen.c"
  "sw/device/lib/dif/autogen/dif_uart_autogen.h"
)

# ------------------------------------------------------------------------------
# Call the OpenTitan driver setup
# ------------------------------------------------------------------------------
include(${CMAKE_SOURCE_DIR}/cmake/opentitan.cmake)

# ------------------------------------------------------------------------------
# Generate the uart_regs.h header using the OT regtool
# ------------------------------------------------------------------------------
set(OPENTITAN_SW_DIR   "${OPENTITAN_DIR}/sw")
set(OPENTITAN_HW_IP_DIR "${OPENTITAN_DIR}/hw/ip")
set(OPENTITAN_UTIL_DIR  "${OPENTITAN_DIR}/util")

set(REGTOOL_PY  "${OPENTITAN_UTIL_DIR}/regtool.py")
set(UART_HJSON  "${OPENTITAN_HW_IP_DIR}/uart/data/uart.hjson")
set(UART_REGS_H "${OPENTITAN_SW_DIR}/device/lib/dif/autogen/uart_regs.h")


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

# Generate the uart_regs.h header using the OT regtool
add_custom_target(
  generate_uart_regs
  ALL
  DEPENDS "${UART_REGS_H}"
)

add_dependencies(generate_uart_regs opentitan_interface_${OPENTITAN_COMMIT_HASH})

# ------------------------------------------------------------------------------
# Add the UART driver library
# ------------------------------------------------------------------------------
add_library(uart_lib STATIC)

file(GLOB_RECURSE UART_SOURCES
  "${OPENTITAN_SW_DIR}/device/lib/base/*.c"
  "${OPENTITAN_SW_DIR}/device/lib/dif/*.c"
)

target_sources(
  uart_lib
  PRIVATE
    ${UART_SOURCES}
    ${UART_REGS_H}
)

target_include_directories(
  uart_lib
  PUBLIC
  ${OPENTITAN_DIR}
  ${OPENTITAN_SW_DIR}/device/lib/dif/autogen
)