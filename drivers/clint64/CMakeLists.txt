# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>

# 64-bit CLINT Driver CMake Configuration

add_library(clint_lib STATIC)

file(GLOB_RECURSE SOURCES
  "src/*.c"
)

target_sources(clint_lib
  PRIVATE
    ${SOURCES}
)

target_include_directories(clint_lib
  PUBLIC
    ${CMAKE_SOURCE_DIR}/hal/inc
    ${CMAKE_SOURCE_DIR}/targets/${TARGET_PLATFORM}/inc
    "inc/"
)

target_link_libraries(clint_lib
  PUBLIC
    runtime_host
)
# Define the macro to indicate a 64-bit CLINT implementation.
target_compile_definitions(clint64 PRIVATE PLATFORM_IS_64BIT)
