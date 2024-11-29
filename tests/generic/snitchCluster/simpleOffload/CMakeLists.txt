# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>
# Philip Wiese <wiesep@iis.ee.ethz.ch>

set(TEST_NAME test_snitchCluster_simpleOffload)

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
  -march=${ISA_HOST}
  -mabi=${ABI_HOST}
)
target_link_libraries(${TEST_NAME}_host PUBLIC chimera-sdk)

######## CLUSTER Code ##########################################################
file(GLOB_RECURSE TEST_SNITCH_SRCS
  "src_cluster/*.c"
)

add_library(${TEST_NAME}_cluster OBJECT ${TEST_SNITCH_SRCS})
target_include_directories(${TEST_NAME}_cluster PUBLIC include)

# WIESEP: Set the correct ISA and ABI for the cluster
target_compile_options(${TEST_NAME}_cluster
  PRIVATE
  -O2
  -march=${ISA_CLUSTER_SNITCH}
  -mabi=${ABI_CLUSTER_SNITCH}
)
target_link_libraries(${TEST_NAME}_cluster PUBLIC chimera-sdk)

######## TEST Executable #######################################################
add_chimera_test(
  ${TEST_NAME}
)

# WIESEP: Link the host and cluster code to the test executable (chimera-sdk is already linked)
target_link_libraries(${TEST_NAME} PUBLIC ${TEST_NAME}_host)
target_link_libraries(${TEST_NAME} PUBLIC ${TEST_NAME}_cluster)
