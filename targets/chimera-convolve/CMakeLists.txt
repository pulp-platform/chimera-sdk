# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>
# Philip Wiese <wiesep@iis.ee.ethz.ch>

file(GLOB_RECURSE ASM_SOURCES
  "src/crt0.S"
)

file(GLOB_RECURSE C_SOURCES
  "src/*.c"
)

set_property(SOURCE ${ASM_SOURCES} PROPERTY LANGUAGE ASM)

target_sources(runtime
  PRIVATE
  ${ASM_SOURCES}
  ${C_SOURCES}
)

# WIESEP: Export the target specific linkerscript
target_link_options(runtime
  PUBLIC
  -L${CMAKE_CURRENT_LIST_DIR}
  -Tlink.ld
)

# WIESEP: Export the target specific include directory
target_include_directories(runtime
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
)


