# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna.
# SPDX-License-Identifier: Apache-2.0

################################################################################
# Target-specific paths for multi-binary compilation                           #
################################################################################

set(CHIMERA_TARGET_DIR
    ${CMAKE_CURRENT_LIST_DIR}
    CACHE PATH "Directory containing target-specific files for Chimera Host" FORCE
)

################################################################################
# Host Runtime Configuration                                                    #
################################################################################
set_property(GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/host/src/crt0.S
)
set_property(GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/shared/inc
    ${CMAKE_CURRENT_LIST_DIR}/shared/inc/regs
    ${PROJECT_SOURCE_DIR}/host/hal/inc
)
set_property(GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_DEFINES
    TARGET_PLATFORM_CHIMERA_HOST
    OT_PLATFORM_RV32
)
set_property(GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_PRIVATE_INCLUDES "")

################################################################################
# Snitch Cluster Runtime Library                                               #
################################################################################
# WIESEP: There is no Snitch cluster present in this target
set_property(GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_SOURCES "")
set_property(GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_INCLUDES "")
set_property(GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_DEFINES "")
set_property(GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_PRIVATE_INCLUDES "")
