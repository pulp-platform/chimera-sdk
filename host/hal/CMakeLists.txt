# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set_property(GLOBAL APPEND PROPERTY CHIMERA_HOST_RUNTIME_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/inc)

set_property(GLOBAL APPEND PROPERTY CHIMERA_HOST_RUNTIME_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/device_api.c
    ${CMAKE_CURRENT_LIST_DIR}/src/interface_api.c
)
