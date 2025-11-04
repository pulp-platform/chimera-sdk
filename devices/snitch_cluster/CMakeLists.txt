# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

################################################################################
# Tramploline Functions
################################################################################
file(GLOB_RECURSE C_SOURCES_SNITCH
  "trampoline_snitchCluster.c"
)
target_sources(trampoline_rv32im PRIVATE ${C_SOURCES_SNITCH})
target_include_directories(trampoline_rv32im PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(runtime_cluster_snitch PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(runtime)
add_subdirectory(third_party)
