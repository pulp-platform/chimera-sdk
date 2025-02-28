# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Sergio Mazzola <smazzola@iis.ee.ethz.ch>

################################################################################
# Pulp Cluster Runtime Library                                               #
################################################################################
file(GLOB_RECURSE C_SOURCES_PULP
  "trampoline_pulpCluster.c"
)

target_sources(runtime_cluster_pulp PRIVATE ${C_SOURCES_PULP})
target_include_directories(runtime_cluster_pulp PUBLIC ${CMAKE_CURRENT_LIST_DIR})
