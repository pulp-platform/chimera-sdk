# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Philip Wiese <wiesep@iis.ee.ethz.ch>

# Define mappings directly using lists for each target
# set(CHIMERA_DEVICE_CONVOLVE_FOLDERS snitch_cluster)
# set(CHIMERA_DEVICE_OPEN_FOLDERS snitch_cluster)
# set(CHIMERA_DEVICE_HOST_FOLDERS)

# # Determine which folders to include based on TARGET_PLATFORM
# if(TARGET_PLATFORM STREQUAL "chimera-convolve")
#     set(INCLUDED_FOLDERS ${CHIMERA_DEVICE_CONVOLVE_FOLDERS})
# elseif(TARGET_PLATFORM STREQUAL "chimera-open")
#     set(INCLUDED_FOLDERS ${CHIMERA_DEVICE_OPEN_FOLDERS})
# elseif(TARGET_PLATFORM STREQUAL "chimera-host")
#     set(INCLUDED_FOLDERS ${CHIMERA_DEVICE_HOST_FOLDERS})
# endif()

# # WIESEP: Print the folders being included
# message(STATUS "[CHIMERA-SDK] Enabled Devices       : ${INCLUDED_FOLDERS}")

# # Add subdirectories, checking for a valid CMakeLists.txt in each folder
# foreach(folder IN LISTS INCLUDED_FOLDERS)
#     if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${folder}/CMakeLists.txt)
#         add_subdirectory(${folder})
#     else()
#         message(WARNING "[CHIMERA-SDK] Device folder '${folder}' does not contain a valid CMakeLists.txt. Skipping.")
#     endif()
# endforeach()

# Define mappings for devices
set(DEVICE_MAPPINGS
    chimera-convolve:snitch_cluster
    chimera-open:snitch_cluster
    chimera-host:
)

# Call the macro
add_chimera_subdirectories(${TARGET_PLATFORM} "Device" DEVICE_MAPPINGS)
