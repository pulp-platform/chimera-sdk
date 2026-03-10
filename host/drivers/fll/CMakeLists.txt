# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0


################################################################################
# Host Runtime Library                                                         #
################################################################################
file(GLOB_RECURSE C_SOURCES
  "fll.c"
)

set_property(GLOBAL APPEND PROPERTY CHIMERA_HOST_RUNTIME_SOURCES ${C_SOURCES})
set_property(GLOBAL APPEND PROPERTY CHIMERA_HOST_RUNTIME_DEFINES CHIMERA_DRIVER_FLL)
