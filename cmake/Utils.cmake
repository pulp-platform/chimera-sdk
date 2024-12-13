# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>

macro(add_chimera_executable name)
  add_executable(${ARGV})
  add_custom_command(
    TARGET ${name}
    POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -dhS $<TARGET_FILE:${name}> > $<TARGET_FILE:${name}>.s)
endmacro()

## TODO: Add vsim target or some such
macro(add_chimera_test name)
  add_chimera_executable(${ARGV})
  add_test(NAME ${name} COMMAND ${name})
endmacro()

macro(add_target_source name)
  if(NOT ${name} IN_LIST AVAILABLE_TARGETS)
    message(FATAL_ERROR "Invalid value for TARGET_PLATFORM: Got ${TARGET_PLATFORM}")
  endif()

  if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${name})
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/${name})
  else()
    message(WARNING "Path ${CMAKE_CURRENT_LIST_DIR}/${name} does not exist")
  endif()
endmacro()


#[=======================================================================[.rst:
.. command:: add_chimera_subdirectories(target_platform, category, mappings)

   Add subdirectories based on a mapping of target platforms to folders.
   The mappings are expected to be in the format ``target_platform:folder1,folder2,...``.

   :param target_platform: The target platform to build for.
   :param category: The category of the subdirectories.
   :param mappings: A list of mappings from target platforms to folders. Make sure to wrap the list in quotes!
   
   .. code-block:: cmake
      :caption Example Usage

      set(MAPPINGS
          chimera-convolve:snitch_cluster
          chimera-open:snitch_cluster
          chimera-host:
      )
      add_chimera_subdirectories(${TARGET_PLATFORM} "Device" "${MAPPINGS}")

#]=======================================================================]
function(add_chimera_subdirectories target_platform category mappings)
  # Initialize included folders
  set(included_folders "")

  # Process mappings
  foreach(mapping IN LISTS mappings)
    string(FIND "${mapping}" ":" delim_pos)
    if(delim_pos EQUAL -1)
      message(WARNING "[CHIMERA-SDK] Invalid mapping entry: '${mapping}'. Skipping.")
      continue()
    endif()

    # Extract key and value
    string(SUBSTRING "${mapping}" 0 ${delim_pos} key)
    math(EXPR value_start "${delim_pos} + 1")
    string(SUBSTRING "${mapping}" ${value_start} -1 value)

    if(key STREQUAL "${target_platform}")
      list(APPEND included_folders ${value})
      break()
    endif()
  endforeach()

  string(REPLACE "," ";" included_folders "${included_folders}")

  # Align output with padding
  string(LENGTH "[CHIMERA-SDK] Enabled ${category}s" category_prefix_length)
  math(EXPR padding_length "36 - ${category_prefix_length}")
  if(padding_length GREATER 0)
    string(REPEAT " " ${padding_length} padding)
  else()
    set(padding "")
  endif()

  # Debug: Print the folders being included
  message(STATUS "[CHIMERA-SDK] Enabled ${category}s${padding}: ${included_folders}")

  # Add subdirectories, checking for a valid CMakeLists.txt
  foreach(folder IN LISTS included_folders)
    if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${folder}/CMakeLists.txt)
      add_subdirectory(${folder})
    else()
      message(WARNING "[CHIMERA-SDK] ${category} folder '${folder}' does not contain a valid CMakeLists.txt. Skipping.")
    endif()
  endforeach()
endfunction()
