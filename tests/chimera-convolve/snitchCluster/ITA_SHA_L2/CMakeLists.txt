# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set(TEST_NAME test_snitchCluster_ITA_SHA_L2)

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
  "src_cluster/test_cluster.c"
  # "src_cluster/ITA_mem_S64_E128_P64.c"
  "src_cluster/ITA_mem_S128_E128_P64.c"
  # "src_cluster/ITA_mem_S256_E384_P64.c"
)

add_library(${TEST_NAME}_cluster OBJECT ${TEST_SNITCH_SRCS})
target_include_directories(${TEST_NAME}_cluster PUBLIC include)

# WIESEP: Use -O1 optimization level for the cluster code otherwise the code currently breaks.
target_compile_options(${TEST_NAME}_cluster
  PRIVATE
  -O2
)

target_link_libraries(${TEST_NAME}_cluster PUBLIC runtime_cluster_snitch)

######## TEST Executable #######################################################
add_chimera_test(
  ${TEST_NAME}
)

# WIESEP: Link the host and cluster code to the test executable (chimera-sdk is already linked)
target_link_libraries(${TEST_NAME} PUBLIC ${TEST_NAME}_host)
target_link_libraries(${TEST_NAME} PUBLIC ${TEST_NAME}_cluster)
