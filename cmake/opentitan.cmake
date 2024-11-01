# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>

include(ExternalProject)

set(OPENTITAN_DIR ${CMAKE_CURRENT_BINARY_DIR}/../drivers/opentitan)

if(EXISTS ${OPENTITAN_DIR})
  file(REMOVE_RECURSE ${OPENTITAN_DIR})
endif()

set(SPARSE_CHECKOUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/sparse-checkout)

set(SPARSE_PATTERNS
  "util/*"
  "hw/ip/*"
  "sw/device/lib/base/*"
  "sw/device/lib/base/internal/*"
  "sw/device/lib/dif/*"
  "sw/device/lib/dif/autogen/*"
)

file(WRITE ${SPARSE_CHECKOUT_FILE} "")
foreach(PATTERN ${SPARSE_PATTERNS})
  file(APPEND ${SPARSE_CHECKOUT_FILE} "${PATTERN}\n")
endforeach()

# Add OpenTitan as an external project
ExternalProject_Add(
  opentitan
  DOWNLOAD_COMMAND
    git clone --no-checkout https://github.com/lowRISC/opentitan.git ${OPENTITAN_DIR}
    COMMAND git -C ${OPENTITAN_DIR} config core.sparseCheckout true
    COMMAND ${CMAKE_COMMAND} -E copy ${SPARSE_CHECKOUT_FILE} ${OPENTITAN_DIR}/.git/info/sparse-checkout
    COMMAND git -C ${OPENTITAN_DIR} checkout master
  UPDATE_COMMAND ""  # Disable updates to avoid unexpected changes
  CONFIGURE_COMMAND ""  # No configure step needed
  BUILD_COMMAND ""  # No build step needed
  INSTALL_COMMAND ""  # No install step needed
  SOURCE_DIR ${OPENTITAN_DIR}
)

message(STATUS "OpenTitan added as an external project.")

set(OPENTITAN_SW_DIR ${OPENTITAN_DIR}/sw)
set(OPENTITAN_HW_IP_DIR ${OPENTITAN_DIR}/hw/ip)
set(OPENTITAN_UTIL_DIR ${OPENTITAN_DIR}/util)

set(REGTOOL_PY ${OPENTITAN_UTIL_DIR}/regtool.py)
set(UART_HJSON ${OPENTITAN_HW_IP_DIR}/uart/data/uart.hjson)
set(UART_REGS_H ${OPENTITAN_SW_DIR}/device/lib/dif/autogen/uart_regs.h)

add_custom_command(
  OUTPUT ${UART_REGS_H}
  COMMAND ${Python3_EXECUTABLE} ${REGTOOL_PY} ${UART_HJSON} -D > ${UART_REGS_H}
  DEPENDS ${REGTOOL_PY} ${UART_HJSON}
  COMMENT "Generating uart_regs.h"
)

add_custom_target(
  generate_uart_regs ALL
  DEPENDS ${UART_REGS_H}
)

add_dependencies(generate_uart_regs opentitan)

ExternalProject_Get_Property(opentitan SOURCE_DIR)

file(GLOB_RECURSE OPENTITAN_SOURCES
    "${OPENTITAN_SW_DIR}/device/lib/base/*.c"
    "${OPENTITAN_SW_DIR}/device/lib/dif/*.c"
    "${OPENTITAN_SW_DIR}/device/lib/dif/autogen/*.c"
    "${OPENTITAN_SW_DIR}/device/lib/runtime/*.c"
)

list(FILTER OPENTITAN_SOURCES EXCLUDE REGEX ".*crc32.*")
list(FILTER OPENTITAN_SOURCES EXCLUDE REGEX ".*hardened.*")

message(STATUS "OpenTitan sources collected.")

add_library(opentitan_lib STATIC ${OPENTITAN_SOURCES})

add_dependencies(opentitan_lib opentitan generate_uart_regs)

target_include_directories(opentitan_lib
    PUBLIC
    ${OPENTITAN_SW_DIR}
    ${OPENTITAN_SW_DIR}/device/lib
    ${OPENTITAN_SW_DIR}/device/lib/base
    ${OPENTITAN_SW_DIR}/device/lib/base/internal
    ${OPENTITAN_SW_DIR}/device/lib/dif
    ${OPENTITAN_SW_DIR}/device/lib/dif/autogen
)

target_link_libraries(opentitan_lib
    PUBLIC
    runtime
)

message(STATUS "OpenTitan library configured.")

