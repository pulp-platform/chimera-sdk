# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

################################################################################
# Snitch Cluster Device                                                        #
################################################################################
chimera_add_device_runtime(snitch_cluster INCLUDES ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(runtime)
add_subdirectory(third_party)
