# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Philip Wiese <wiesep@iis.ee.ethz.ch>

file(GLOB_RECURSE C_SOURCES_SNTICH
  "trampoline_snitchCluster.c"
)

file(GLOB_RECURSE C_SOURCES
  "*.c"
)

# WIESEP: Remove the C_SOURCES_SNTICH from the list of sources
list(REMOVE_ITEM C_SOURCES ${C_SOURCES_SNTICH})

# WIESEP: Create an object library for the snitch cluster trampoline to compile it with the correct ISA and ABI
add_library(cluster_snitch OBJECT ${C_SOURCES_SNTICH})

target_compile_options(cluster_snitch
  PRIVATE
  -O2
  -march=${ISA_CLUSTER_SNITCH}
  -mabi=${ABI_CLUSTER_SNITCH}
)

# Add include directories from runtime to cluster_snitch
target_include_directories(cluster_snitch
  PRIVATE
  $<TARGET_PROPERTY:runtime,INTERFACE_INCLUDE_DIRECTORIES>
)

target_sources(runtime INTERFACE $<TARGET_OBJECTS:cluster_snitch>)
target_sources(runtime PRIVATE ${C_SOURCES})



