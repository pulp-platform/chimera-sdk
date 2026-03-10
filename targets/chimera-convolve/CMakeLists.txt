# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

################################################################################
# Target-specific paths for multi-binary compilation                           #
################################################################################

set(CHIMERA_TARGET_DIR
    ${CMAKE_CURRENT_LIST_DIR}
    CACHE PATH "Directory containing target-specific files for Chimera Convolve" FORCE
)

################################################################################
# Host Runtime Configuration                                                    #
################################################################################
set_property(GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/host/src/padframe_gpio.c
    ${CMAKE_CURRENT_LIST_DIR}/host/src/padframe_hyperbus.c
    ${CMAKE_CURRENT_LIST_DIR}/host/src/test.c
    ${CMAKE_CURRENT_LIST_DIR}/host/src/crt0.S
    ${CMAKE_CURRENT_LIST_DIR}/shared/src/cluster_addr_map.c
    ${CMAKE_CURRENT_LIST_DIR}/shared/src/shared.c
)
set_property(GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/host/inc
    ${CMAKE_CURRENT_LIST_DIR}/shared/inc
    ${CMAKE_CURRENT_LIST_DIR}/shared/inc/regs
    ${CMAKE_CURRENT_LIST_DIR}/shared/inc/addr_maps
    ${PROJECT_SOURCE_DIR}/host/hal/inc
)
set_property(GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_DEFINES
    TARGET_PLATFORM_CHIMERA_CONVOLVE
    OT_PLATFORM_RV32
)
set_property(GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_PRIVATE_INCLUDES "")

################################################################################
# Device (Snitch Cluster) Runtime Configuration                                 #
################################################################################
set_property(GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/devices/snitch_cluster/src/trampoline.c
    ${CMAKE_CURRENT_LIST_DIR}/devices/snitch_cluster/src/symbols.c
    ${CMAKE_CURRENT_LIST_DIR}/devices/snitch_cluster/src/crt0.S
    ${CMAKE_CURRENT_LIST_DIR}/shared/src/cluster_addr_map.c
    ${CMAKE_CURRENT_LIST_DIR}/shared/src/shared.c
)
set_property(GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/devices/snitch_cluster/inc
    ${CMAKE_CURRENT_LIST_DIR}/shared/inc
    ${CMAKE_CURRENT_LIST_DIR}/shared/inc/regs
    ${CMAKE_CURRENT_LIST_DIR}/shared/inc/cfg
    ${CMAKE_CURRENT_LIST_DIR}/shared/inc/addr_maps
)
set_property(GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_DEFINES
    TARGET_PLATFORM_CHIMERA_CONVOLVE
)
set_property(GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_PRIVATE_INCLUDES "")
