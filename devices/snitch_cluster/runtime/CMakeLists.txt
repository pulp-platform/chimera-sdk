# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

################################################################################
# Snitch Cluster Runtime Library                                               #
################################################################################
file(GLOB C_SOURCES
  "putc.c"
  "init.c"
  "stdio.c"
)

chimera_add_device_runtime(snitch_cluster
    SOURCES  ${C_SOURCES}
    DEFINES  CHIMERA_DEVICE_SNITCH_CLUSTER
    INCLUDES ${CMAKE_CURRENT_LIST_DIR}
)
