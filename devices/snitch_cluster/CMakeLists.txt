# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

################################################################################
# Snitch Cluster Device                                                        #
################################################################################
set_property(GLOBAL APPEND PROPERTY CHIMERA_DEVICE_RUNTIME_INCLUDES ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(runtime)
add_subdirectory(third_party)
