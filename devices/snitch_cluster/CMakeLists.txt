# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Philip Wiese <wiesep@iis.ee.ethz.ch>


################################################################################
# Snitch Cluster Runtime Library                                               #
################################################################################
file(GLOB_RECURSE C_SOURCES_SNITCH
  "trampoline_snitchCluster.c"
)

target_sources(runtime_cluster_snitch PRIVATE ${C_SOURCES_SNITCH})
