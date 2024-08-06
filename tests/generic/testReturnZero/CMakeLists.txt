# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_COMPILER_WORKS 1)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/toolchain_gcc.cmake)

project(chimera-sdk-test-build LANGUAGES C ASM)

file(GLOB_RECURSE TEST_SRCS
  "src/testReturn0.c"
)

add_chimera_test(
  return0
  ${TEST_SRCS}
)

target_link_libraries(return0 PUBLIC chimera-sdk)
