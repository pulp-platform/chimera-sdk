# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Philip Wiese <wiesep@iis.ee.ethz.ch>

################################################################################
# Snitch Cluster Runtime Library                                               #
################################################################################
file(GLOB C_SOURCES
  "putc.c"
  "init.c"
)

target_sources(runtime_cluster_snitch PRIVATE ${C_SOURCES})
target_compile_definitions(runtime_cluster_snitch PUBLIC CHIMERA_DEVICE_SNITCH_CLUSTER)

target_include_directories(runtime_cluster_snitch PUBLIC ${CMAKE_CURRENT_LIST_DIR})
