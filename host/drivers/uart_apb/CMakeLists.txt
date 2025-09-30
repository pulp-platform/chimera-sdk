# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>

# APB UART Driver CMake Configuration

add_library(uart_lib STATIC)

file(GLOB_RECURSE SOURCES
  "src/*.c"
)

target_sources(uart_lib
  PRIVATE
    ${SOURCES}
)

target_include_directories(uart_lib
  PUBLIC
    ${PROJECT_SOURCE_DIR}/host/hal/inc
    "inc/"
)

target_link_libraries(uart_lib
  PUBLIC
    runtime_host
)

