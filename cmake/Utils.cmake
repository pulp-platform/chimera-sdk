# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

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
  if(TEST_MODE STREQUAL "simulation")
    add_test(NAME ${name} COMMAND ${SIMULATION_BINARY} +BINARY=$<TARGET_FILE:${name}> +PRELMODE=${PRELOAD_MODE_INT})
  endif()
endmacro()

#[=======================================================================[.rst:
.. cmake:command:: chimera_select_shape(OPTION <var> DEFAULT <shape> HEADER_PREFIXES <prefix>...)

   Select which data shape a test is built for.

   Tests that ship several problem sizes keep one header per shape (and
   optionally one source per shape). This picks exactly one of them at
   configure time instead of requiring includes to be commented in and out.

   The available shapes are discovered from the files on disk matching the
   first entry of ``HEADER_PREFIXES``, so adding ``<prefix><shape>.h`` to the
   test is enough to make a new shape selectable.

   :param OPTION: Name of the cache variable exposed to the user. Required.
   :param DEFAULT: Shape to use when the user does not set ``OPTION``. Required.
   :param HEADER_PREFIXES: Filename prefixes of the per-shape headers. The first
      is used to discover the available shapes; every prefix must resolve to an
      existing header for the selected shape. Required.
   :param HEADER_DIR: Directory holding the headers. Defaults to ``include``.
   :param SOURCE_PREFIX: If given, also select ``<SOURCE_DIR>/<prefix><shape>.c``.
   :param SOURCE_DIR: Directory holding the sources. Defaults to ``src_cluster``.

   Sets in the caller's scope:

   ``SELECTED_SHAPE``
      The chosen shape, e.g. ``64x128x64``.
   ``SELECTED_SHAPE_SOURCE``
      The matching source file, or empty if no ``SOURCE_PREFIX`` was given.

   .. code-block:: cmake
      :caption: Example Usage

      chimera_select_shape(
        OPTION          ITA_MATMUL_L1_SHAPE
        DEFAULT         64x128x64
        HEADER_PREFIXES ITA_dims_
        SOURCE_PREFIX   ITA_mem_
      )

#]=======================================================================]
function(chimera_select_shape)
  set(oneValueArgs OPTION DEFAULT HEADER_DIR SOURCE_DIR SOURCE_PREFIX)
  set(multiValueArgs HEADER_PREFIXES)
  cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(NOT ARG_OPTION OR NOT ARG_DEFAULT OR NOT ARG_HEADER_PREFIXES)
    message(FATAL_ERROR
      "[CHIMERA-SDK] chimera_select_shape requires OPTION, DEFAULT and HEADER_PREFIXES")
  endif()

  if(NOT ARG_HEADER_DIR)
    set(ARG_HEADER_DIR include)
  endif()
  if(NOT ARG_SOURCE_DIR)
    set(ARG_SOURCE_DIR src_cluster)
  endif()

  # Discover the selectable shapes from the headers carrying the first prefix
  list(GET ARG_HEADER_PREFIXES 0 discovery_prefix)
  set(header_root ${CMAKE_CURRENT_SOURCE_DIR}/${ARG_HEADER_DIR})
  file(GLOB headers RELATIVE ${header_root} ${header_root}/${discovery_prefix}*.h)

  set(shapes "")
  foreach(header IN LISTS headers)
    string(REGEX REPLACE "^${discovery_prefix}(.+)\\.h$" "\\1" shape "${header}")
    list(APPEND shapes ${shape})
  endforeach()
  list(SORT shapes)

  if(NOT shapes)
    message(FATAL_ERROR
      "[CHIMERA-SDK] No ${discovery_prefix}*.h headers found in ${header_root}")
  endif()

  set(${ARG_OPTION} ${ARG_DEFAULT} CACHE STRING
      "Data shape to build ${ARG_OPTION} for. One of: ${shapes}")
  set_property(CACHE ${ARG_OPTION} PROPERTY STRINGS ${shapes})
  set(shape ${${ARG_OPTION}})

  if(NOT shape IN_LIST shapes)
    message(FATAL_ERROR
      "[CHIMERA-SDK] Invalid value for ${ARG_OPTION}: got '${shape}'. "
      "Available shapes: ${shapes}")
  endif()

  # Every prefix must resolve for the selected shape, otherwise the dimension
  # macros and the data they describe would silently disagree.
  foreach(prefix IN LISTS ARG_HEADER_PREFIXES)
    if(NOT EXISTS ${header_root}/${prefix}${shape}.h)
      message(FATAL_ERROR
        "[CHIMERA-SDK] Shape '${shape}' selected by ${ARG_OPTION} is missing "
        "${header_root}/${prefix}${shape}.h")
    endif()
  endforeach()

  set(source "")
  if(ARG_SOURCE_PREFIX)
    set(source ${CMAKE_CURRENT_SOURCE_DIR}/${ARG_SOURCE_DIR}/${ARG_SOURCE_PREFIX}${shape}.c)
    if(NOT EXISTS ${source})
      message(FATAL_ERROR
        "[CHIMERA-SDK] Shape '${shape}' selected by ${ARG_OPTION} is missing ${source}")
    endif()
  endif()

  message(STATUS "[CHIMERA-SDK] ${ARG_OPTION}: ${shape}")

  set(SELECTED_SHAPE        ${shape}  PARENT_SCOPE)
  set(SELECTED_SHAPE_SOURCE ${source} PARENT_SCOPE)
endfunction()

macro(add_target_source name)
  if(NOT ${name} IN_LIST AVAILABLE_TARGETS)
    message(FATAL_ERROR "[CHIMERA-SDK] Invalid value for TARGET_PLATFORM: Got ${TARGET_PLATFORM}")
  endif()

  if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${name})
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/${name})
  else()
    message(WARNING "[CHIMERA-SDK] Path ${CMAKE_CURRENT_LIST_DIR}/${name} does not exist")
  endif()
endmacro()


#[=======================================================================[.rst:
.. cmake:command:: add_chimera_subdirectories(target_platform, category, mappings)

   Add subdirectories based on a mapping of target platforms to folders.
   The mappings are expected to be in the format ``target_platform:folder1,folder2,...``.
   :param target_platform: The target platform to build for.
   :param category: The category of the subdirectories.
   :param mappings: A list of mappings from target platforms to folders. Make sure to wrap the list in quotes!

   .. code-block:: cmake
      :caption: Example Usage

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
    string(SUBSTRING "${mapping}" 0 ${delim_pos} key_raw)
    math(EXPR value_start "${delim_pos} + 1")
    string(SUBSTRING "${mapping}" ${value_start} -1 value_raw)

    # Normalize whitespace around key/value (handles multi-line values)
    string(STRIP "${key_raw}"   key)
    string(STRIP "${value_raw}" value)

    # Accept commas across newlines and optional spaces "a,\n  b,  c" -> "a;b;c"
    string(REGEX REPLACE "[ \t\r\n]*,[ \t\r\n]*" ";" value "${value}")

    if(key STREQUAL "${target_platform}")
      list(APPEND included_folders ${value})
      break()
    endif()
  endforeach()

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
