# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>
# Philip Wiese <wiesep@iis.ee.ethz.ch>

# Define mappings for drivers
set(DRIVER_MAPPINGS
    chimera-convolve:cluster
    chimera-open:cluster
    chimera-host:
)

# Call the macro
add_chimera_subdirectories(${TARGET_PLATFORM} "Driver" "${DRIVER_MAPPINGS}")


# WIESEP: Export this directory as root include directory for the drivers
target_include_directories(runtime_host PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# VIVIANEP: Link to the opentitan library
target_link_libraries(runtime_host
    PUBLIC
    opentitan_lib
)