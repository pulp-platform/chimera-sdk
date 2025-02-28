# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>
# Philip Wiese <wiesep@iis.ee.ethz.ch>
# Sergio Mazzola <smazzola@iis.ee.ethz.ch>

################################################################################
# Host Runtime Library                                                         #
################################################################################
add_library(runtime_host STATIC)

file(GLOB_RECURSE ASM_SOURCES
  "src/crt0.S"
)

file(GLOB_RECURSE C_SOURCES
  "src/*.c"
)

set_property(SOURCE ${ASM_SOURCES} PROPERTY LANGUAGE ASM)

target_sources(runtime_host
  PRIVATE
  ${ASM_SOURCES}
  ${C_SOURCES}
)

# WIESEP: Export the target specific include directory
target_include_directories(runtime_host
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
)

target_compile_options(runtime_host
  PRIVATE
  -O2
)

target_compile_options(runtime_host
  PUBLIC
  -march=${ISA_HOST}
  -mabi=${ABI}
)

# WIESEP: Export the target specific linkerscript
target_link_options(runtime_host
  PUBLIC
  -L${CMAKE_CURRENT_LIST_DIR}
  -Tlink.ld
  -march=${ISA_HOST}
  -mabi=${ABI}
  -nostartfiles
  -ffreestanding
)

################################################################################
# PULP Cluster Runtime Library                                                 #
################################################################################
add_library(runtime_cluster_pulp STATIC)

# WIESEP: Export the headers also to the cluster runtime
target_include_directories(runtime_cluster_pulp
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
)

# WIESEP: Do not export optimization flags
target_compile_options(runtime_cluster_pulp
  PRIVATE
  -O2
)

target_compile_options(runtime_cluster_pulp
  PUBLIC
  -march=${ISA_CLUSTER_PULP}
  -mabi=${ABI}
)

# WIESEP: Expose common link option
target_link_options(runtime_cluster_pulp
  PUBLIC
  -march=${ISA_CLUSTER_PULP}
  -mabi=${ABI}
  -nostartfiles
  -ffreestanding
)