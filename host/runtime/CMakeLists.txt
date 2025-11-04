# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0


target_include_directories(runtime_host
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/inc
)

if(HARDWARE_BACKEND STREQUAL "RTL")
  target_compile_definitions(runtime_host PUBLIC HARDWARE_BACKEND_RTL)
elseif(HARDWARE_BACKEND STREQUAL "GVSoC")
  target_compile_definitions(runtime_host PUBLIC HARDWARE_BACKEND_GVSOC)
elseif(HARDWARE_BACKEND STREQUAL "ASIC")
  target_compile_definitions(runtime_host PUBLIC HARDWARE_BACKEND_ASIC)
endif()

target_sources(runtime_host PRIVATE
  src/putc.c
  src/alloc.c
  src/stdio.c
  src/uart.c
  src/clint.c
  src/log.c
)
