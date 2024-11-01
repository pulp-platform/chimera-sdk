# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>

include(ExternalProject)

set(OPENTITAN_DIR ${CMAKE_CURRENT_BINARY_DIR}/../drivers/opentitan)

# Add OpenTitan as an external project
ExternalProject_Add(
  opentitan
  GIT_REPOSITORY https://github.com/lowRISC/opentitan.git
  GIT_TAG master  # Consider specifying a specific tag or commit for stability
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
