# Copyright 2025 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Victor Jung <jungvi@iis.ee.ethz.ch>

target_include_directories(runtime_host
  PRIVATE 
    ${PROJECT_SOURCE_DIR}/hal/inc
    ${PROJECT_SOURCE_DIR}/generic_runtime/inc
)


option(RTL_SIM "Enable RTL simulation backend" OFF)

if(RTL_SIM)
message(STATUS "Using RTL UART backend")
target_sources(runtime_host PRIVATE
src/putc_rtl.c
src/uart_init_rtl.c
src/uart_deinit_rtl.c
)
else()
message(STATUS "Using GVSOC UART backend")
target_sources(runtime_host PRIVATE 
src/putc_gvsoc.c
src/uart_init_gvsoc.c 
src/uart_deinit_gvsoc.c
)
endif()

target_sources(runtime_host PRIVATE 
  src/alloc.c
  src/stdio.c
)

add_dependencies(runtime_host picolibc)