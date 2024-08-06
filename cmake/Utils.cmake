# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>

macro(add_chimera_executable name)
  add_executable(${ARGV})
  add_custom_command(
    TARGET ${name}
    POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -dhS $<TARGET_FILE:${name}> > $<TARGET_FILE:${name}>.s)
endmacro()

## TODO: Add vsim target or some such
macro(add_chimera_test name)
  add_chimera_executable(${ARGV})
  add_test(NAME ${name} COMMAND ${name})
endmacro()
