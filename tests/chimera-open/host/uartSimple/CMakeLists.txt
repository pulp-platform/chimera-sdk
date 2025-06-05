# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>

set(TEST_NAME test_host_uartSimple)

######## HOST Code #############################################################
file(GLOB_RECURSE TEST_HOST_SRCS
  "src_host/*.c"
)

add_library(${TEST_NAME}_host OBJECT ${TEST_HOST_SRCS})
target_include_directories(${TEST_NAME}_host PUBLIC 
    inc 
    ${PROJECT_SOURCE_DIR}/devices/snitch_cluster
)

# VIVIANEP: Include trampoline source to resolve the _trampoline symbol used by the host.
target_sources(${TEST_NAME}_host PRIVATE ${PROJECT_SOURCE_DIR}/devices/snitch_cluster/trampoline_snitchCluster.c)

# WIESEP: Set the correct ISA and ABI for the host
target_compile_options(${TEST_NAME}_host PRIVATE -O2)
target_link_libraries(${TEST_NAME}_host PUBLIC runtime_host hal_host)

######## TEST Executable #######################################################
add_chimera_test(${TEST_NAME})
target_link_libraries(${TEST_NAME} PUBLIC ${TEST_NAME}_host)
