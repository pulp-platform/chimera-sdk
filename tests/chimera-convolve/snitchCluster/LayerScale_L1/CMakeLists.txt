# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set(TEST_NAME test_snitchCluster_LayerScale_L1)

######## Test Vectors ##########################################################
chimera_test_vector_generator(
  GENERATOR ${CMAKE_CURRENT_SOURCE_DIR}/generate_vectors.py
  OUTPUTS
            ${CMAKE_CURRENT_SOURCE_DIR}/include/testdata_16x384.h
)

######## HOST Code #############################################################
file(GLOB_RECURSE TEST_HOST_SRCS
  "src_host/*.c"
)

add_library(${TEST_NAME}_host OBJECT ${TEST_HOST_SRCS})
target_include_directories(${TEST_NAME}_host PUBLIC include)

# WIESEP: Set the correct ISA and ABI for the host
target_compile_options(${TEST_NAME}_host
  PRIVATE
  -O2
)
target_link_libraries(${TEST_NAME}_host PUBLIC hal_host runtime_host picolibc_host trampoline_rv32im)

######## CLUSTER Code ##########################################################
file(GLOB_RECURSE TEST_SNITCH_SRCS
  "src_cluster/*.c"
)

add_library(${TEST_NAME}_cluster OBJECT ${TEST_SNITCH_SRCS})
target_include_directories(${TEST_NAME}_cluster PUBLIC include)

target_compile_options(${TEST_NAME}_cluster
  PRIVATE
  -O2
)
target_link_libraries(${TEST_NAME}_cluster PUBLIC runtime_cluster_snitch picolibc_cluster_snitch)

######## TEST Executable #######################################################
add_chimera_test(
  ${TEST_NAME}
)

# WIESEP: Link the host and cluster code to the test executable
target_link_libraries(${TEST_NAME} PUBLIC ${TEST_NAME}_host)
target_link_libraries(${TEST_NAME} PUBLIC ${TEST_NAME}_cluster)

