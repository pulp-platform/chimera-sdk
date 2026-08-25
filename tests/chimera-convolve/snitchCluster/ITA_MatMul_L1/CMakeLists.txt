# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set(TEST_NAME test_snitchCluster_ITA_MatMul_L1)

######## Matrix Shape ##########################################################
# Pick the SxExP shape to build for, e.g. -DITA_MATMUL_L1_SHAPE=64x512x64
chimera_select_shape(
  OPTION          ITA_MATMUL_L1_SHAPE
  DEFAULT         64x128x64
  HEADER_PREFIXES ITA_dims_
  SOURCE_PREFIX   ITA_mem_
)

######## HOST Code #############################################################
file(GLOB_RECURSE TEST_HOST_SRCS
  "src_host/*.c"
)

add_library(${TEST_NAME}_host OBJECT ${TEST_HOST_SRCS})
target_include_directories(${TEST_NAME}_host PUBLIC include)
# The host sources pull in test_cluster.h, so they need the same shape selection
target_compile_definitions(${TEST_NAME}_host PUBLIC ITA_DIMS_HEADER="ITA_dims_${SELECTED_SHAPE}.h")

# WIESEP: Set the correct ISA and ABI for the host
target_compile_options(${TEST_NAME}_host
  PRIVATE
  -O2
)
target_link_libraries(${TEST_NAME}_host PUBLIC hal_host runtime_host picolibc_host trampoline_rv32im)

######## CLUSTER Code ##########################################################
set(TEST_SNITCH_SRCS
  "src_cluster/test_cluster.c"
  ${SELECTED_SHAPE_SOURCE}
)

add_library(${TEST_NAME}_cluster OBJECT ${TEST_SNITCH_SRCS})
target_include_directories(${TEST_NAME}_cluster PUBLIC include)
target_compile_definitions(${TEST_NAME}_cluster PUBLIC ITA_DIMS_HEADER="ITA_dims_${SELECTED_SHAPE}.h")

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
