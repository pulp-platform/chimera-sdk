# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>
# Philip Wiese <wiesep@iis.ee.ethz.ch>

# Define mappings directly using lists for each target
set(CHIMERA_CONVOLVE_FOLDERS chimera-convolve chimera-open chimera-host)
set(CHIMERA_OPEN_FOLDERS chimera-open chimera-host)
set(CHIMERA_HOST_FOLDERS chimera-host)

# Determine which folders to include based on TARGET_PLATFORM
if(TARGET_PLATFORM STREQUAL "chimera-convolve")
    set(INCLUDED_FOLDERS ${CHIMERA_CONVOLVE_FOLDERS})
elseif(TARGET_PLATFORM STREQUAL "chimera-open")
    set(INCLUDED_FOLDERS ${CHIMERA_OPEN_FOLDERS})
elseif(TARGET_PLATFORM STREQUAL "chimera-host")
    set(INCLUDED_FOLDERS ${CHIMERA_HOST_FOLDERS})
else()
    message(FATAL_ERROR "[CHIMERA-SDK] Unknown TARGET_PLATFORM: '${TARGET_PLATFORM}'. Valid options are chimera-convolve, chimera-open, or chimera-host.")
endif()

# WIESEP: Print the folders being included
message(STATUS "[CHIMERA-SDK] Including test folders: ${INCLUDED_FOLDERS}")

# Add subdirectories, checking for a valid CMakeLists.txt in each folder
foreach(folder IN LISTS INCLUDED_FOLDERS)
    if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${folder}/CMakeLists.txt)
        add_subdirectory(${folder})
    else()
        message(WARNING "[CHIMERA-SDK] Folder '${folder}' does not contain a valid CMakeLists.txt. Skipping.")
    endif()
endforeach()
