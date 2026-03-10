# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

include(ExternalProject)

set(CROSS_C_COMPILER "${CMAKE_C_COMPILER}")
set(CROSS_C_ARGS "--target=${CROSS_COMPILE_HOST} -nostdlib -ggdb -gdwarf-4 -gstrict-dwarf ${CMAKE_ALT_C_OPTIONS}")
set(CROSS_C_LINK_ARGS "--target=${CROSS_COMPILE_HOST} -nostdlib -fno-common -Wl,-z,noexecstack -fuse-ld=lld")

set(CROSS_AR "${CMAKE_AR}")
set(CROSS_STRIP "${CMAKE_STRIP}")

set(CROSS_CPU "${HOST_ARCH}")
set(CROSS_CPU_FAMILY "${HOST_FAMILY}")
set(CROSS_ENDIAN "${HOST_ENDIAN}")
set(CROSS_SYSTEM "${HOST_SYSTEM}")

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

prepare_meson_array(CROSS_C_ARGS_LIST "${CROSS_C_ARGS}")
prepare_meson_array(CROSS_C_LINK_ARGS_LIST "${CROSS_C_LINK_ARGS}")

set(PICOLIBC_SRC_DIR ${CMAKE_BINARY_DIR}/picolibc-src)

set(PICOLIBC_BUILD_DIR ${CMAKE_BINARY_DIR}/picolibc-build)
set(PICOLIBC_INSTALL_DIR ${CMAKE_BINARY_DIR}/picolibc-install)
set(PICOLIBC_CROSS_FILE ${CMAKE_BINARY_DIR}/picolibc-cross-file.txt)

# Generate the Meson cross-file
configure_file(${CMAKE_CURRENT_LIST_DIR}/../scripts/picolibc-cross-file.txt.in ${PICOLIBC_CROSS_FILE} @ONLY)

set(PICOLIB_MULTILIB ${PICOLIB_HOST})
if(NOT ${PICOLIB_CLUSTER_SNITCH} STREQUAL "None")
    list(APPEND PICOLIB_MULTILIB ${PICOLIB_CLUSTER_SNITCH})
endif()
string(JOIN "," PICOLIB_MULTILIB ${PICOLIB_MULTILIB})

# Add picolibc as an external project
ExternalProject_Add(
    picolibc
    GIT_REPOSITORY https://github.com/picolibc/picolibc.git
    GIT_TAG 1.8.11
    SOURCE_DIR ${PICOLIBC_SRC_DIR}
    BINARY_DIR ${PICOLIBC_BUILD_DIR}
    INSTALL_DIR ${PICOLIBC_INSTALL_DIR}
    CONFIGURE_COMMAND meson setup ${PICOLIBC_BUILD_DIR} ${PICOLIBC_SRC_DIR} --cross-file ${PICOLIBC_CROSS_FILE} -D multilib-list=${PICOLIB_MULTILIB} --prefix ${PICOLIBC_INSTALL_DIR} --default-library=static
    BUILD_COMMAND ninja -C ${PICOLIBC_BUILD_DIR}
    INSTALL_COMMAND ninja -C ${PICOLIBC_BUILD_DIR} install
    BUILD_BYPRODUCTS
    ${PICOLIBC_INSTALL_DIR}/lib/${PICOLIB_HOST}/libc.a
    ${PICOLIBC_INSTALL_DIR}/lib/${PICOLIB_CLUSTER_SNITCH}/libc.a
    # LOG_CONFIGURE ON
    # LOG_BUILD ON
    LOG_INSTALL ON
)

set(PICOLIBC_TARGET picolibc)

################################################################################
# Host Picolibc Library
################################################################################
add_library(picolibc_host STATIC IMPORTED GLOBAL)
set_target_properties(picolibc_host PROPERTIES
    IMPORTED_LOCATION "${PICOLIBC_INSTALL_DIR}/lib/${PICOLIB_HOST}/libc.a"
)
add_dependencies(picolibc_host picolibc)

################################################################################
# Snitch Picolibc Library
################################################################################
if(${ISA_CLUSTER_SNITCH} STREQUAL "NONE")
    return()
else()
    add_library(picolibc_cluster_snitch STATIC IMPORTED GLOBAL)
    set_target_properties(picolibc_cluster_snitch PROPERTIES
        IMPORTED_LOCATION "${PICOLIBC_INSTALL_DIR}/lib/${PICOLIB_CLUSTER_SNITCH}/libc.a"
    )
    add_dependencies(picolibc_cluster_snitch picolibc)
endif()

