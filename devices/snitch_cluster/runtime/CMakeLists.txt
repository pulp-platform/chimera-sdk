# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

################################################################################
# Snitch Cluster Runtime Library                                               #
################################################################################
file(GLOB C_SOURCES
  "putc.c"
  "init.c"
  "stdio.c"
)

set_property(GLOBAL APPEND PROPERTY CHIMERA_DEVICE_RUNTIME_SOURCES ${C_SOURCES})
set_property(GLOBAL APPEND PROPERTY CHIMERA_DEVICE_RUNTIME_DEFINES CHIMERA_DEVICE_SNITCH_CLUSTER)
set_property(GLOBAL APPEND PROPERTY CHIMERA_DEVICE_RUNTIME_INCLUDES ${CMAKE_CURRENT_LIST_DIR})
