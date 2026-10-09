# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

if(NOT DEFINED CHIMERA_BINARY_DIR)
    message(FATAL_ERROR "[CHIMERA] CHIMERA_BINARY_DIR is not set")
endif()

if(NOT DEFINED CHIMERA_LISTS_FILE OR NOT EXISTS "${CHIMERA_LISTS_FILE}")
    message(FATAL_ERROR
        "[CHIMERA] CHIMERA_LISTS_FILE not set or does not exist: '${CHIMERA_LISTS_FILE}'")
endif()

include("${CHIMERA_LISTS_FILE}")

list(LENGTH SECTION_FILES _n_files)
list(LENGTH BINARY_NAMES  _n_bins)

if(NOT _n_files EQUAL _n_bins)
    message(FATAL_ERROR
        "[CHIMERA] SECTION_FILES and BINARY_NAMES must have the same length")
endif()

message(STATUS "[CHIMERA] =========================================")
message(STATUS "[CHIMERA] Build Summary")
message(STATUS "[CHIMERA] =========================================")

if(_n_bins GREATER 0)
    math(EXPR _last_idx "${_n_bins} - 1")
    foreach(_i RANGE ${_last_idx})
        list(GET BINARY_NAMES  ${_i} _bin)
        list(GET SECTION_FILES ${_i} _sections_file)

        get_filename_component(_bin_dir "${_sections_file}" DIRECTORY)

        if(_bin_dir MATCHES "/host($|/)")
            set(_role "Host")
        else()
            set(_role "Device ${_bin}")
        endif()

        message(STATUS "[CHIMERA]   ${_role} Binary:             ${_bin_dir}/${_bin}.elf")
        message(STATUS "[CHIMERA]   ${_role} Disassembly:        ${_bin_dir}/${_bin}.dump")
        message(STATUS "[CHIMERA]   ${_role} Sections:           ${_sections_file}")
        message(STATUS "[CHIMERA]   ${_role} Symbols:            ${_bin_dir}/${_bin}.symbols")

        set(_export_header "${_bin_dir}/${_bin}_symbols.h")
        if(EXISTS "${_export_header}")
            message(STATUS "[CHIMERA]   ${_role} Export Header:      ${_export_header}")
        endif()

        set(_placement_file "${CHIMERA_BINARY_DIR}/${_bin}_placement.ldh")
        if(EXISTS "${_placement_file}")
            message(STATUS "[CHIMERA]   ${_role} Placement:          ${_placement_file}")
        endif()

        if(NOT _i EQUAL _last_idx)
            message(STATUS "[CHIMERA] -----------------------------------------")
        endif()
    endforeach()
endif()

# Optional: show the unified ELF when it was generated
if(DEFINED CHIMERA_UNIFIED_ELF AND CHIMERA_UNIFIED_ELF)
    message(STATUS "[CHIMERA] -----------------------------------------")
    message(STATUS "[CHIMERA]   Unified ELF:         ${CHIMERA_UNIFIED_ELF_PATH}.elf")
    message(STATUS "[CHIMERA]   Unified Disassembly: ${CHIMERA_UNIFIED_ELF_PATH}.dump")
    message(STATUS "[CHIMERA]   Unified Sections:    ${CHIMERA_UNIFIED_ELF_PATH}.sections")
    message(STATUS "[CHIMERA]   Unified Symbols:     ${CHIMERA_UNIFIED_ELF_PATH}.symbols")
endif()

message(STATUS "[CHIMERA] =========================================")
