# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

# Define mappings for drivers
set(DRIVER_MAPPINGS
    chimera-convolve:cluster,uart_apb,clint32,fll,hyperbus
    chimera-open:cluster,uart_apb,clint32
    chimera-host:uart_apb,clint32
)

# Call the macro
add_chimera_subdirectories(${TARGET_PLATFORM} "Driver" "${DRIVER_MAPPINGS}")

# WIESEP: Export this directory as root include directory for the drivers
target_include_directories(runtime_host PUBLIC ${CMAKE_CURRENT_LIST_DIR})
