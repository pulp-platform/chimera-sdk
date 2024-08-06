# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>

cmake_minimum_required(VERSION 3.13)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/Utils.cmake)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_TOOLCHAIN_FILE cmake/toolchain_gcc.cmake)

project(chimera-sdk LANGUAGES C ASM)

add_subdirectory(targets)
add_subdirectory(hal)

add_library(chimera-sdk INTERFACE)
target_link_libraries(chimera-sdk INTERFACE hal)
target_link_libraries(chimera-sdk INTERFACE runtime)
target_sources(chimera-sdk INTERFACE $<TARGET_OBJECTS:runtime>)

enable_testing()

add_subdirectory(tests)
