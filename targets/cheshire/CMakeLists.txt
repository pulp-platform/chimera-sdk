# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>

file(GLOB_RECURSE ASM_SOURCES
  "src/crt0.S"
)
set_property(SOURCE ${ASM_SOURCES} PROPERTY LANGUAGE ASM)
add_library(runtime OBJECT ${ASM_SOURCES})

set(ISA rv32imc)
set(ABI ilp32)

target_compile_options(runtime
  PUBLIC
  -march=${ISA}
  -mabi=${ABI}
)

target_link_options(runtime
  PUBLIC
  -march=${ISA}
  -mabi=${ABI}
  -nostartfiles
  -nostdlib
  -L${CMAKE_CURRENT_LIST_DIR}
  -Tlink.ld
)
