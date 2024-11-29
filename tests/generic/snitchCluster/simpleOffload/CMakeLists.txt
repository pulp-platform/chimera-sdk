# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>
# Philip Wiese <wiesep@iis.ee.ethz.ch>

file(GLOB_RECURSE TEST_SRCS
  "src/test.c"
)

add_chimera_test(
  test_snitchCluster_simpleOffload
  ${TEST_SRCS}
)

target_link_libraries(test_snitchCluster_simpleOffload PUBLIC chimera-sdk)
