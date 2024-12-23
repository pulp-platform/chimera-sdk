# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>

include(ExternalProject)

message(STATUS "[CHIMERA-SDK] Setting up picolibc for target ${TARGET}")

foreach(var CROSS_C_COMPILER CROSS_C_COMPILER_ARGS CROSS_AR CROSS_STRIP CROSS_CPU CROSS_CPU_FAMILY CROSS_ENDIAN CROSS_SYSTEM CROSS_C_ARGS CROSS_C_LINK_ARGS)
    if(NOT DEFINED ${var})
        message(FATAL_ERROR "[CHIMERA-SDK] Variable ${var} is not set. It must be defined before including picolibc.cmake.")
    endif()
endforeach()

# Prepare Meson arrays
function(prepare_meson_array output_var input_string)
    string(REPLACE " " ";" temp_list "${input_string}")
    set(formatted_list "")
    foreach(item IN LISTS temp_list)
        list(APPEND formatted_list "'${item}'")
    endforeach()
    string(JOIN ", " result ${formatted_list})
    set(${output_var} "${result}" PARENT_SCOPE)
endfunction()

prepare_meson_array(CROSS_C_COMPILER_ARGS_LIST "${CROSS_C_COMPILER_ARGS}")
prepare_meson_array(CROSS_C_ARGS_LIST "${CROSS_C_ARGS}")
prepare_meson_array(CROSS_C_LINK_ARGS_LIST "${CROSS_C_LINK_ARGS}")

set(PICOLIBC_SRC_DIR ${CMAKE_BINARY_DIR}/picolibc-src)
set(PICOLIBC_BUILD_DIR ${CMAKE_BINARY_DIR}/picolibc-build-${TARGET})
set(PICOLIBC_INSTALL_DIR ${CMAKE_BINARY_DIR}/picolibc-install-${TARGET})
set(PICOLIBC_CROSS_FILE ${CMAKE_BINARY_DIR}/picolibc-cross-file-${TARGET}.txt)


# Generate the Meson cross-file
configure_file(${CMAKE_CURRENT_LIST_DIR}/../scripts/picolibc-cross-file.txt.in ${PICOLIBC_CROSS_FILE} @ONLY)

message(STATUS "[CHIMERA-SDK] Saving cross compilation file to ${PICOLIBC_CROSS_FILE}")
# Add picolibc as an external project
ExternalProject_Add(
    picolibc-${TARGET}
    GIT_REPOSITORY https://github.com/picolibc/picolibc.git
    GIT_TAG main
    SOURCE_DIR ${PICOLIBC_SRC_DIR}
    BINARY_DIR ${PICOLIBC_BUILD_DIR}
    INSTALL_DIR ${PICOLIBC_INSTALL_DIR}
    CONFIGURE_COMMAND meson setup ${PICOLIBC_BUILD_DIR} ${PICOLIBC_SRC_DIR} --cross-file ${PICOLIBC_CROSS_FILE} --prefix ${PICOLIBC_INSTALL_DIR} --default-library=static
    BUILD_COMMAND ninja -C ${PICOLIBC_BUILD_DIR}
    INSTALL_COMMAND ninja -C ${PICOLIBC_BUILD_DIR} install
)

set(PICOLIBC_INSTALL_DIR ${PICOLIBC_INSTALL_DIR} PARENT_SCOPE)
set(PICOLIBC_TARGET picolibc-${TARGET} PARENT_SCOPE)

add_library(picolibc STATIC IMPORTED GLOBAL)

set_target_properties(picolibc PROPERTIES
    IMPORTED_LOCATION "${PICOLIBC_INSTALL_DIR}/lib/libc.a"
)

add_dependencies(picolibc picolibc-${TARGET})