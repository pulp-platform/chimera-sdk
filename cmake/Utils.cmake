# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

macro(add_chimera_executable name)
  add_executable(${ARGV})
  add_custom_command(
    TARGET ${name}
    POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -dhS $<TARGET_FILE:${name}> > $<TARGET_FILE:${name}>.s
    COMMAND ${CMAKE_NM} -S $<TARGET_FILE:${name}> > $<TARGET_FILE:${name}>.map
  )
  set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES
    $<TARGET_FILE:${name}>.s
    $<TARGET_FILE:${name}>.map
  )
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

   Pick one data shape for a test at configure time. Shapes are discovered from
   the headers matching the first ``HEADER_PREFIXES`` entry, so adding
   ``<prefix><shape>.h`` is enough to make a new shape selectable.

   Sets ``SELECTED_SHAPE`` and ``SELECTED_SHAPE_SOURCE`` in the caller's scope.

   :param OPTION: Cache variable exposed to the user. Required.
   :param DEFAULT: Shape used when ``OPTION`` is unset. Required.
   :param HEADER_PREFIXES: Prefixes of the per-shape headers. The first discovers
      the shapes; all must exist for the selected one. Required.
   :param HEADER_DIR: Where the headers live. Defaults to ``include``.
   :param SOURCE_PREFIX: If given, also select ``<SOURCE_DIR>/<prefix><shape>.c``.
   :param SOURCE_DIR: Where the sources live. Defaults to ``src_cluster``.

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
      "[CHIMERA-SDK] No ${discovery_prefix}*.h headers found in:\n"
      "    ${header_root}\n"
      "  Test vectors are not tracked in git. Generate them with:\n"
      "    python scripts/generate_test_vectors.py")
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

  # All prefixes must resolve, else the dimension macros and the data disagree
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
        "[CHIMERA-SDK] Shape '${shape}' selected by ${ARG_OPTION} is missing:\n"
        "    ${source}\n"
        "  Test vectors are not tracked in git. Generate them with:\n"
        "    python scripts/generate_test_vectors.py")
    endif()
  endif()

  message(STATUS "[CHIMERA-SDK] ${ARG_OPTION}: ${shape}")

  set(SELECTED_SHAPE        ${shape}  PARENT_SCOPE)
  set(SELECTED_SHAPE_SOURCE ${source} PARENT_SCOPE)
endfunction()

#[=======================================================================[.rst:
.. cmake:command:: chimera_test_vector_generator(GENERATOR <script> OUTPUTS <file>...)

   Register a test's ``generate_vectors.py``. Fails configure with an actionable
   message when the vectors have not been generated yet, and adds them to the
   ``regenerate-test-vectors`` target.

   :param GENERATOR: Path to the generator script. Required.
   :param OUTPUTS: Files the generator writes. Required. Used for the
      missing-vector check and as the target's byproducts.
   :param ARGS: Optional generator arguments. Omit when the generator already
      defaults to the right paths, which is the usual case.

#]=======================================================================]
function(chimera_test_vector_generator)
  set(oneValueArgs GENERATOR)
  set(multiValueArgs OUTPUTS ARGS)
  cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(NOT ARG_GENERATOR OR NOT ARG_OUTPUTS)
    message(FATAL_ERROR
      "[CHIMERA-SDK] chimera_test_vector_generator requires GENERATOR and OUTPUTS")
  endif()
  if(NOT EXISTS ${ARG_GENERATOR})
    message(FATAL_ERROR "[CHIMERA-SDK] Generator ${ARG_GENERATOR} does not exist")
  endif()

  # Only needed when a generator runs; resolve lazily and cache for later callers
  if(NOT DEFINED CHIMERA_VECTOR_PYTHON)
    find_package(Python3 QUIET COMPONENTS Interpreter)
    set(CHIMERA_VECTOR_PYTHON "${Python3_EXECUTABLE}" CACHE INTERNAL
        "Interpreter used to run test-vector generators")
  endif()

  # One global target that refreshes every registered generator in-tree
  if(NOT TARGET regenerate-test-vectors)
    add_custom_target(regenerate-test-vectors
      COMMENT "[CHIMERA-SDK] Regenerating test vectors")
  endif()

  # Fail here rather than letting the compiler report a missing header
  foreach(required IN LISTS ARG_OUTPUTS)
    if(NOT EXISTS ${required})
      message(FATAL_ERROR
        "[CHIMERA-SDK] Missing generated test vector:\n"
        "    ${required}\n"
        "  Test vectors are not tracked in git. Generate them with:\n"
        "    python scripts/generate_test_vectors.py\n"
        "  (add --skip-ita to build only the tests that need no ITA checkout)")
    endif()
  endforeach()

  # Name the target after its location, not the absolute path, so it stays
  # readable in `cmake --build --target help`
  file(RELATIVE_PATH rel ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
  if(NOT rel)
    get_filename_component(rel ${ARG_GENERATOR} NAME_WE)
  endif()
  string(MAKE_C_IDENTIFIER "${rel}" stem)
  add_custom_target(regenerate_${stem}
    COMMAND ${CHIMERA_VECTOR_PYTHON} ${ARG_GENERATOR} ${ARG_ARGS}
    BYPRODUCTS ${ARG_OUTPUTS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "[CHIMERA-SDK] Regenerating vectors for ${rel}"
    VERBATIM)
  add_dependencies(regenerate-test-vectors regenerate_${stem})
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

# Generic command to dissassemble a target
macro(disassemble_target target)
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -dhS $<TARGET_FILE:${target}> > $<TARGET_FILE:${target}>.s
    COMMAND ${CMAKE_NM} -S $<TARGET_FILE:${target}> > $<TARGET_FILE:${target}>.map
    )
  set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES
    $<TARGET_FILE:${target}>.s
    $<TARGET_FILE:${target}>.map
  )
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
