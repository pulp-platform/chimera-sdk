# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

################################################################################
# Host Runtime Library                                                         #
################################################################################
file(GLOB_RECURSE C_SOURCES
  "uart_apb.c"
)

target_sources(runtime_host PRIVATE ${C_SOURCES})
target_compile_definitions(runtime_host PUBLIC CHIMERA_DRIVER_UART_APB)
target_compile_definitions(runtime_host PUBLIC CHIMERA_DRIVER_UART)

