# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

# 32-bit CLINT Driver CMake Configuration

file(GLOB_RECURSE C_SOURCES
  "clint32.c"
)

target_sources(runtime_host PRIVATE ${C_SOURCES})
target_compile_definitions(runtime_host PUBLIC CHIMERA_DRIVER_CLINT32)
target_compile_definitions(runtime_host PUBLIC CHIMERA_DRIVER_CLINT)
