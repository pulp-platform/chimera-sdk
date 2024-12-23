# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>

include(ExternalProject)

# ------------------------------------------------------------------------------
# Validate prerequisites
# ------------------------------------------------------------------------------
if(NOT DEFINED OPENTITAN_COMMIT_HASH)
  message(FATAL_ERROR "[CHIMERA-SDK] Please set OPENTITAN_COMMIT_HASH in your target.")
endif()

if(NOT DEFINED OPENTITAN_SPARSE_PATTERNS)
    message(FATAL_ERROR "[CHIMERA-SDK] Please set OPENTITAN_SPARSE_PATTERNS for your target drivers.")
endif()

if(NOT DEFINED OPENTITAN_DIR)
  message(FATAL_ERROR "[CHIMERA-SDK] Please set OPENTITAN_DIR in your target driver.")
endif()

# ------------------------------------------------------------------------------
# Write sparse checkout file
# ------------------------------------------------------------------------------
set(SPARSE_CHECKOUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/sparse-checkout")
file(WRITE "${SPARSE_CHECKOUT_FILE}" "")
foreach(PATTERN ${OPENTITAN_SPARSE_PATTERNS})
  file(APPEND "${SPARSE_CHECKOUT_FILE}" "${PATTERN}\n")
endforeach()

# ------------------------------------------------------------------------------
# Clone logic
# ------------------------------------------------------------------------------
if(NOT EXISTS "${OPENTITAN_DIR}/.git")
  execute_process(
    COMMAND git clone --no-checkout https://github.com/lowRISC/opentitan.git "${OPENTITAN_DIR}"
    RESULT_VARIABLE GIT_CLONE_RESULT
    ERROR_VARIABLE GIT_CLONE_ERROR
  )
  if(NOT GIT_CLONE_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to clone OpenTitan repository: ${GIT_CLONE_ERROR}")
  endif()
else()
  message(STATUS "[CHIMERA-SDK] OpenTitan repository already exists at ${OPENTITAN_DIR}")
endif()

# ------------------------------------------------------------------------------
# Configure sparse checkout
# ------------------------------------------------------------------------------
execute_process(
  COMMAND git -C "${OPENTITAN_DIR}" config core.sparseCheckout true
  RESULT_VARIABLE GIT_CONFIG_RESULT
  OUTPUT_VARIABLE GIT_CONFIG_OUTPUT
)

if(NOT GIT_CONFIG_RESULT EQUAL 0)
  message(FATAL_ERROR "Failed to configure sparse checkout: ${GIT_CONFIG_OUTPUT}")
endif()

file(COPY "${SPARSE_CHECKOUT_FILE}" DESTINATION "${OPENTITAN_DIR}/.git/info")

# ------------------------------------------------------------------------------
# Apply sparse checkout
# ------------------------------------------------------------------------------
execute_process(
  COMMAND git -C "${OPENTITAN_DIR}" read-tree -mu HEAD
  RESULT_VARIABLE GIT_READ_TREE_RESULT
  OUTPUT_VARIABLE GIT_READ_TREE_OUTPUT
)

if(NOT GIT_READ_TREE_RESULT EQUAL 0)
  message(FATAL_ERROR "Failed to apply sparse checkout: ${GIT_READ_TREE_OUTPUT}")
endif()

# ------------------------------------------------------------------------------
# Checkout the desired commit hash
# ------------------------------------------------------------------------------
execute_process(
  COMMAND git -C "${OPENTITAN_DIR}" checkout "${OPENTITAN_COMMIT_HASH}"
  RESULT_VARIABLE GIT_CHECKOUT_RESULT
  OUTPUT_VARIABLE GIT_CHECKOUT_OUTPUT
)

if(NOT GIT_CHECKOUT_RESULT EQUAL 0)
  message(FATAL_ERROR "Failed to checkout commit ${OPENTITAN_COMMIT_HASH}: ${GIT_CHECKOUT_OUTPUT}")
endif()

# ------------------------------------------------------------------------------
# Add OpenTitan as an external project
# ------------------------------------------------------------------------------
ExternalProject_Add(
  opentitan_build_${OPENTITAN_COMMIT_HASH}
  SOURCE_DIR "${OPENTITAN_DIR}"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  BUILD_BYPRODUCTS "${OPENTITAN_DIR}/.git/config"
  BUILD_ALWAYS 1
)

add_custom_target(opentitan_target_${OPENTITAN_COMMIT_HASH} ALL
  DEPENDS opentitan_build_${OPENTITAN_COMMIT_HASH}
)

# ------------------------------------------------------------------------------
# Interface target for other targets to depend on
# ------------------------------------------------------------------------------
add_library(opentitan_interface_${OPENTITAN_COMMIT_HASH} INTERFACE)
add_dependencies(opentitan_interface_${OPENTITAN_COMMIT_HASH} opentitan_target_${OPENTITAN_COMMIT_HASH})
