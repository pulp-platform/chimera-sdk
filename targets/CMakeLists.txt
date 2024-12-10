# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>

set(AVAILABLE_TARGETS
  "chimera-host"
  "chimera-open"
  "chimera-convolve"
  CACHE STRING "Available Targets"
)

get_property(OPT_STRINGS CACHE OPT PROPERTY STRINGS)

if(NOT TARGET_PLATFORM IN_LIST AVAILABLE_TARGETS)
  message(FATAL_ERROR "Wrong value for TARGET_PLATFORM: Got ${TARGET_PLATFORM}. Available targets are: ${AVAILABLE_TARGETS}")
endif()

message(STATUS "[CHIMERA-SDK] TARGET_PLATFORM       : ${TARGET_PLATFORM}")

if (TARGET_PLATFORM STREQUAL "chimera-host")
  add_subdirectory(chimera-host)
endif()

if (TARGET_PLATFORM STREQUAL "chimera-open")
  add_subdirectory(chimera-open)
endif()

if (TARGET_PLATFORM STREQUAL "chimera-convolve")
  add_subdirectory(chimera-convolve)
endif()
