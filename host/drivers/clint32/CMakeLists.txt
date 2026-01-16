# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>

# 32-bit CLINT Driver CMake Configuration

file(GLOB_RECURSE C_SOURCES
  "clint32.c"
)

target_sources(runtime_host PRIVATE ${C_SOURCES})
target_compile_definitions(runtime_host PUBLIC CHIMERA_DRIVER_CLINT32)
target_compile_definitions(runtime_host PUBLIC CHIMERA_DRIVER_CLINT)
