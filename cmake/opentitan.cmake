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

add_library(opentitan_lib STATIC IMPORTED GLOBAL)

set_target_properties(opentitan_lib PROPERTIES
  IMPORTED_LOCATION ${OPENTITAN_DIR}/sw/device/lib/libot.a
)

add_dependencies(opentitan_lib opentitan)
