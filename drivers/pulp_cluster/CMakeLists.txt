# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Sergio Mazzola <smazzola@iis.ee.ethz.ch>


################################################################################
# Host Runtime Library                                                         #
################################################################################
file(GLOB_RECURSE C_SOURCES
  "*.c"
)

target_sources(runtime_host PRIVATE ${C_SOURCES})
target_compile_definitions(runtime_host INTERFACE CHIMERA_DRIVER_PULP_CLUSTER)

target_include_directories(runtime_host PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
