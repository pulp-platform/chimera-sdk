# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set(TEST_NAME test_host_returnZero)

######## HOST Code #############################################################
file(GLOB_RECURSE TEST_HOST_SRCS
  "src_host/*.c"
)

add_library(${TEST_NAME}_host OBJECT ${TEST_HOST_SRCS})
target_include_directories(${TEST_NAME}_host PUBLIC
  inc
  ${PROJECT_SOURCE_DIR}/devices
)

# WIESEP: Set the correct ISA and ABI for the host
target_compile_options(${TEST_NAME}_host PRIVATE -O2)
target_link_libraries(${TEST_NAME}_host PUBLIC runtime_host)

######## TEST Executable #######################################################
add_chimera_test(${TEST_NAME})
target_link_libraries(${TEST_NAME} PUBLIC ${TEST_NAME}_host)

################################################################################
# Debugging Options                                                             #
################################################################################

target_compile_options(${TEST_NAME}_host
  PUBLIC
  -ffunction-sections
  -fdata-sections
)

target_link_options(${TEST_NAME}
  PUBLIC
  # -Wl,--print-memory-usage
  -Wl,--print-gc-sections
  -Wl,--gc-sections
)

