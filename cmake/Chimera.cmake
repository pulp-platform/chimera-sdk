# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

#[=======================================================================[.rst:
.. cmake:command:: add_device_binary(TARGET_NAME)

   Compile a device binary for one execution domain (e.g. a Snitch cluster)
   and expose its public symbols for the host to link against.

   This function drives the first two stages of the three-stage heterogeneous
   compilation pipeline:

   - **Stage 1** — Compile the device ELF with the device ISA/ABI.
   - **Stage 2** — Extract public symbols with ``llvm-nm`` and write an
     assembly stub (``_symbols.s``) and C header (``_symbols.h``) that the
     host links against.  A placement header (``_placement.ldh``) recording
     the 4 KiB-aligned end address of the device binary is also written.

   The resulting ``${TARGET_NAME}_symbols`` INTERFACE library is consumed by
   :cmake:command:`add_host_binary`.

   :param TARGET_NAME: CMake target name for this device binary.
   :param DEVICE_NAME: Short prefix applied to every exported symbol,
      e.g. ``device`` produces ``device_main``.  Must be unique across all
      devices in a build to avoid host link-time collisions.
   :param TARGET_DIR: Directory containing ``common.ldh`` and other
      target-wide shared files (e.g. ``targets/<target>/``).
   :param DEVICE_DIR: Directory containing the device linker script template
      ``link.ld.in`` and device-specific startup code
      (e.g. ``targets/<target>/devices/snitch_cluster/``).
   :param ISA: RISC-V march string for the device (e.g. ``rv32imafd``).
   :param ABI: RISC-V mabi string for the device (e.g. ``ilp32d``).
   :param COMPILER: LLVM target triple for the device
      (e.g. ``riscv32-unknown-elf``).
   :param COMPIERT_RT: compiler-rt baremetal subdirectory for this ABI
      (e.g. ``rv32imafd``).
   :param SOURCES: C and assembly source files compiled into the device ELF.
   :param PREV_DEVICE: *(optional)* CMake target name of the preceding device
      in memisl order.  When set, the generated linker script INCLUDEs that
      device's placement header so this binary starts immediately after it.
   :param PICOLIBC: *(optional)* Picolibc library variant to link
      (e.g. ``rv32imafd``).

   Linker script template placeholders substituted by this function:

   - ``@CHIMERA_PREV_INCLUDE@`` — empty, or ``INCLUDE <prev>_placement.ldh``
   - ``@CHIMERA_RESERVED_SECTION@`` — empty, or a NOLOAD ``.reserved`` section
     advancing the location counter past the previous device's memory region

   Build outputs (all in ``CMAKE_CURRENT_BINARY_DIR``):

   - ``${TARGET_NAME}.elf`` — device ELF
   - ``${TARGET_NAME}.dump`` — disassembly
   - ``${TARGET_NAME}.sections`` — section headers (input for overlap checker)
   - ``${TARGET_NAME}.symbols`` — full symbol table
   - ``${TARGET_NAME}_symbols.s`` — absolute-address assembly for the host linker
   - ``${TARGET_NAME}_symbols.h`` — ``extern`` declarations for host C code
   - ``${TARGET_NAME}_symbols`` — INTERFACE library consumed by :cmake:command:`add_host_binary`
   - ``${TARGET_NAME}_placement.ldh`` — placement header read by the next binary

   .. code-block:: cmake
      :caption: Example Usage

      add_device_binary(${TEST_NAME}_device
          DEVICE_NAME  device
          ISA          ${ISA_CLUSTER_SNITCH}
          ABI          ${ABI_CLUSTER_SNITCH}
          COMPILER     ${CROSS_COMPILE_DEVICE_SNITCH_CLUSTER}
          COMPIERT_RT  ${COMPILERRT_CLUSTER_SNITCH}
          PICOLIBC     ${PICOLIB_CLUSTER_SNITCH}
          TARGET_DIR   ${CHIMERA_TARGET_DIR}
          DEVICE_DIR   ${CHIMERA_TARGET_DIR}/devices/snitch_cluster
          SOURCES      ${TEST_SNITCH_SRCS}
      )

#]=======================================================================]
function(add_device_binary TARGET_NAME)
    set(oneValueArgs   DEVICE_NAME TARGET_DIR DEVICE_DIR PREV_DEVICE ISA ABI COMPILER COMPIERT_RT PICOLIBC)
    set(multiValueArgs SOURCES)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    message(STATUS "[CHIMERA] Device (${TARGET_NAME})")
    message(STATUS "[CHIMERA]    ISA / ABI    : ${ARG_ISA} / ${ARG_ABI}")
    message(STATUS "[CHIMERA]    Compiler     : ${ARG_COMPILER}")
    message(STATUS "[CHIMERA]    Compiler-rt  : ${ARG_COMPIERT_RT}")
    if(ARG_PICOLIBC)
        message(STATUS "[CHIMERA]    Picolibc     : ${ARG_PICOLIBC}")
    endif()
    if(ARG_PREV_DEVICE)
        message(STATUS "[CHIMERA]    -> placed after: ${ARG_PREV_DEVICE}")
    else()
        message(STATUS "[CHIMERA]    -> first device in chain")
    endif()
    message(STATUS "[CHIMERA] ------------------------------------------------------------")

    # Derive compiler-rt library suffix from ABI
    if(ARG_ABI MATCHES "^ilp32")
        set(_compiert_suffix "riscv32")
    elseif(ARG_ABI MATCHES "^lp64")
        set(_compiert_suffix "riscv64")
    else()
        message(FATAL_ERROR "[CHIMERA] Cannot determine compiler-rt suffix from ABI: ${ARG_ABI}")
    endif()

    # -----------------------------------------------------------------------
    # 1. Generate the linker script from the user's template.
    #    configure_file substitutes @CHIMERA_PREV_INCLUDE@ and
    #    @CHIMERA_RESERVED_SECTION@ at cmake configure time.
    # -----------------------------------------------------------------------
    if(ARG_PREV_DEVICE)
        set(CHIMERA_PREV_INCLUDE
            "INCLUDE ${ARG_PREV_DEVICE}_placement.ldh")
        # Build the .reserved block with string(APPEND) to avoid the semicolon
        # being mis-interpreted as a CMake list separator inside set().
        set(CHIMERA_RESERVED_SECTION "")
        string(APPEND CHIMERA_RESERVED_SECTION "  .reserved (NOLOAD) : ALIGN(0x1000) {\n")
        string(APPEND CHIMERA_RESERVED_SECTION "    . = __${ARG_PREV_DEVICE}_end;\n")
        string(APPEND CHIMERA_RESERVED_SECTION "  } > memisl\n")
    else()
        set(CHIMERA_PREV_INCLUDE "")
        set(CHIMERA_RESERVED_SECTION "")
    endif()

    set(GEN_LINK_LD "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_link.ld")
    configure_file("${ARG_DEVICE_DIR}/link.ld.in" "${GEN_LINK_LD}" @ONLY)
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${GEN_LINK_LD}")

    # -----------------------------------------------------------------------
    # 2. Compile the device ELF
    # -----------------------------------------------------------------------
    add_executable(${TARGET_NAME}.elf)

    target_sources(${TARGET_NAME}.elf PRIVATE ${ARG_SOURCES})

    # Place ELF in the subdirectory and suppress the global .elf suffix
    # (the suffix is already part of the target name).
    set_target_properties(${TARGET_NAME}.elf PROPERTIES
        SUFFIX ""
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

    target_compile_options(${TARGET_NAME}.elf PRIVATE
        --target=${ARG_COMPILER}
        -march=${ARG_ISA}
        -mabi=${ARG_ABI}
        -O2
        -g
        -Wall
        -Wextra
    )

    target_link_options(${TARGET_NAME}.elf PRIVATE
        --target=${ARG_COMPILER}
        -march=${ARG_ISA}
        -mabi=${ARG_ABI}
        -nostdlib
        -T${GEN_LINK_LD}
        -Wl,--build-id=none
        -lclang_rt.builtins-${_compiert_suffix}
    )


    target_link_directories(${TARGET_NAME}.elf PRIVATE
        ${TOOLCHAIN_DIR}/lib/clang/${LLVM_VERSION_MAJOR}/lib/baremetal/${ARG_COMPIERT_RT}
        ${ARG_DEVICE_DIR}
        ${ARG_TARGET_DIR}/shared
        ${CMAKE_CURRENT_BINARY_DIR}           # for *_placement.ldh files
    )

    # LINK_DEPENDS: relink the ELF when the generated linker script changes.
    # The generated script itself is regenerated (via configure_file) whenever
    # the source template changes, so tracking GEN_LINK_LD is sufficient.
    # For devices with a previous device, also track the placement file so
    # the ELF relinks when the previous device's size changes.
    if(ARG_PREV_DEVICE)
        set(PREV_PLACEMENT_LDH "${CMAKE_CURRENT_BINARY_DIR}/${ARG_PREV_DEVICE}_placement.ldh")
        set_target_properties(${TARGET_NAME}.elf PROPERTIES
            LINK_DEPENDS "${GEN_LINK_LD};${PREV_PLACEMENT_LDH}"
        )
        add_dependencies(${TARGET_NAME}.elf ${ARG_PREV_DEVICE}_gen_placement)
    else()
        set_target_properties(${TARGET_NAME}.elf PROPERTIES
            LINK_DEPENDS "${GEN_LINK_LD}"
        )
    endif()

    # -----------------------------------------------------------------------
    # 3. Optional: Link selected picolibc instance
    # -----------------------------------------------------------------------
    if(ARG_PICOLIBC)
        set(_picolibc_device_lib "${PICOLIBC_DIR}/lib/${ARG_PICOLIBC}/libc.a")
        set(_picolibc_device_inc "${PICOLIBC_DIR}/include")

        if(NOT EXISTS "${_picolibc_device_lib}")
            message(FATAL_ERROR
                "[CHIMERA] Picolibc library not found:\n"
                "  ${_picolibc_device_lib}\n"
                "Run:  make picolibc-multilib   (from the chimera-sdk root)\n"
                "Or set PICOLIBC_DIR if picolibc is installed elsewhere.")
        endif()

        add_library(${TARGET_NAME}_picolibc STATIC IMPORTED GLOBAL)
        set_target_properties(${TARGET_NAME}_picolibc  PROPERTIES
            IMPORTED_LOCATION "${_picolibc_device_lib}"
        )
        set_target_properties(${TARGET_NAME}_picolibc  PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${_picolibc_device_inc}"
        )
        target_link_libraries(${TARGET_NAME}.elf PRIVATE ${TARGET_NAME}_picolibc)
    endif()

    # Create per-binary device runtime (compiled with this binary's ISA/ABI)
    get_property(_dev_runtime_sources  GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_SOURCES)
    get_property(_dev_runtime_includes GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_INCLUDES)
    get_property(_dev_runtime_priv_inc GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_PRIVATE_INCLUDES)
    get_property(_dev_runtime_defines  GLOBAL PROPERTY CHIMERA_DEVICE_RUNTIME_DEFINES)
    if(_dev_runtime_sources)
        add_library(${TARGET_NAME}_runtime OBJECT ${_dev_runtime_sources})
        target_compile_options(${TARGET_NAME}_runtime PRIVATE
            --target=${ARG_COMPILER}
            -march=${ARG_ISA}
            -mabi=${ARG_ABI}
            -O2
            -ffunction-sections
            -fdata-sections
        )
        if(_dev_runtime_includes)
            target_include_directories(${TARGET_NAME}_runtime PUBLIC ${_dev_runtime_includes})
        endif()
        if(_dev_runtime_priv_inc)
            target_include_directories(${TARGET_NAME}_runtime PRIVATE ${_dev_runtime_priv_inc})
        endif()
        if(_dev_runtime_defines)
            target_compile_definitions(${TARGET_NAME}_runtime PUBLIC ${_dev_runtime_defines})
        endif()
        target_link_libraries(${TARGET_NAME}_runtime PRIVATE ${TARGET_NAME}_picolibc)
        target_link_libraries(${TARGET_NAME}.elf PRIVATE ${TARGET_NAME}_runtime)
    endif()

    # -----------------------------------------------------------------------
    # 4. Debug artifacts: disassembly + section headers + full symbol table
    # -----------------------------------------------------------------------
    add_custom_command(
        OUTPUT  ${TARGET_NAME}.dump ${TARGET_NAME}.sections ${TARGET_NAME}.symbols
        COMMAND ${CMAKE_OBJDUMP} -S  ${TARGET_NAME}.elf > ${TARGET_NAME}.dump
        COMMAND ${CMAKE_OBJDUMP} -h  ${TARGET_NAME}.elf > ${TARGET_NAME}.sections
        COMMAND ${CMAKE_NM}      -n  ${TARGET_NAME}.elf > ${TARGET_NAME}.symbols
        DEPENDS ${TARGET_NAME}.elf
        COMMENT "[CHIMERA] Disassembling ${TARGET_NAME}"
        VERBATIM
    )

    # -----------------------------------------------------------------------
    # 5. Extract public symbols → assembly stub + C header
    # -----------------------------------------------------------------------
    set(SYMBOLS_ASM "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_symbols.s")
    set(SYMBOLS_H   "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_symbols.h")
    set(HELPERS     "${CMAKE_SOURCE_DIR}/cmake/scripts/ChimeraBuildHelpers.cmake")

    add_custom_command(
        OUTPUT  ${SYMBOLS_ASM} ${SYMBOLS_H}
        COMMAND ${CMAKE_COMMAND}
                    -D CHIMERA_MODE=symbols
                    -D NM=${CMAKE_NM}
                    -D TARGET_NAME=${TARGET_NAME}
                    -D DEVICE_NAME=${ARG_DEVICE_NAME}
                    -D ELF=${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.elf
                    -D OUT_ASM=${SYMBOLS_ASM}
                    -D OUT_H=${SYMBOLS_H}
                    -P ${HELPERS}
        DEPENDS ${TARGET_NAME}.elf ${HELPERS}
        COMMENT "[CHIMERA] Extracting symbols from ${TARGET_NAME}"
        VERBATIM
    )

    add_custom_target(${TARGET_NAME}_gen_symbols
        DEPENDS ${SYMBOLS_ASM} ${SYMBOLS_H}
    )

    # -----------------------------------------------------------------------
    # 5. Generate placement file: read __device_end → write _placement.ldh
    # -----------------------------------------------------------------------
    set(PLACEMENT_LDH "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_placement.ldh")

    add_custom_command(
        OUTPUT  ${PLACEMENT_LDH}
        COMMAND ${CMAKE_COMMAND}
                    -D CHIMERA_MODE=placement
                    -D NM=${CMAKE_NM}
                    -D ELF=${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.elf
                    -D TARGET_NAME=${TARGET_NAME}
                    -D OUT_LDH=${PLACEMENT_LDH}
                    -P ${HELPERS}
        DEPENDS ${TARGET_NAME}.elf ${HELPERS}
        COMMENT "[CHIMERA] Generating placement for ${TARGET_NAME}"
        VERBATIM
    )

    add_custom_target(${TARGET_NAME}_gen_placement
        DEPENDS ${PLACEMENT_LDH}
    )

    # -----------------------------------------------------------------------
    # 6. Compile the symbol assembly with the HOST toolchain.
    #    The symbols are absolute addresses so the host ABI is correct here.
    # -----------------------------------------------------------------------
    add_library(${TARGET_NAME}_syms_obj OBJECT ${SYMBOLS_ASM})

    set_source_files_properties(${SYMBOLS_ASM} PROPERTIES
        GENERATED TRUE
        LANGUAGE  ASM
    )

    target_compile_options(${TARGET_NAME}_syms_obj PRIVATE
        --target=${CROSS_COMPILE_HOST}
        -march=${ISA_HOST}
        -mabi=${ABI_HOST}
        -mcmodel=medany
        -Wno-unused-command-line-argument
    )

    add_dependencies(${TARGET_NAME}_syms_obj ${TARGET_NAME}_gen_symbols)

    # -----------------------------------------------------------------------
    # 7. INTERFACE library consumed by add_host_binary().
    #    target_sources propagates OBJECT files through INTERFACE correctly
    #    (target_link_libraries does not in CMake < 3.24).
    # -----------------------------------------------------------------------
    add_library(${TARGET_NAME}_symbols INTERFACE)
    target_sources(${TARGET_NAME}_symbols INTERFACE
        $<TARGET_OBJECTS:${TARGET_NAME}_syms_obj>
    )
    target_include_directories(${TARGET_NAME}_symbols INTERFACE
        ${CMAKE_CURRENT_BINARY_DIR}
    )

    # -----------------------------------------------------------------------
    # 8. Top-level convenience target.
    #    Also record per-device paths as target properties so that
    #    add_host_binary() can query them by device name without relying on
    #    globally-accumulated lists.
    # -----------------------------------------------------------------------
    add_custom_target(${TARGET_NAME} ALL
        DEPENDS
            ${TARGET_NAME}.dump
            ${TARGET_NAME}.sections
            ${TARGET_NAME}.symbols
            ${TARGET_NAME}_gen_symbols
            ${TARGET_NAME}_gen_placement
    )

    set_property(TARGET ${TARGET_NAME} PROPERTY
        CHIMERA_SECTIONS_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.sections")
    set_property(TARGET ${TARGET_NAME} PROPERTY
        CHIMERA_ELF_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.elf")

    add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin"
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.elf
            "${CMAKE_BINARY_DIR}/bin/${TARGET_NAME}.elf"
        COMMENT "[CHIMERA] Copying ${TARGET_NAME}.elf to build/bin"
        VERBATIM
    )
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_BINARY_DIR}/bin/${TARGET_NAME}.elf")

endfunction()

#[=======================================================================[.rst:
.. cmake:command:: add_host_binary(TARGET_NAME)

   Compile the host binary and link the device symbol stubs produced by
   :cmake:command:`add_device_binary`.

   This function drives Stage 3 of the three-stage heterogeneous compilation
   pipeline: the host ELF is compiled with the host ISA/ABI and linked with
   the absolute-address symbol stubs from every attached device.  A post-build
   step runs the section-overlap checker and, when ``CHIMERA_UNIFIED_ELF`` is
   enabled, merges all ELFs into a single mixed-ISA file for simulation.

   :param TARGET_NAME: CMake target name for this host binary.
   :param TARGET_DIR: Directory containing ``common.ldh`` and other
      target-wide shared files (e.g. ``targets/<target>/``).
   :param DEVICE_DIR: Directory containing the host linker script template
      ``link.ld.in`` and host-specific startup code
      (e.g. ``targets/<target>/host/``).
   :param ISA: RISC-V march string for the host (e.g. ``rv64imc``).
   :param ABI: RISC-V mabi string for the host (e.g. ``lp64``).
   :param COMPILER: LLVM target triple for the host
      (e.g. ``riscv64-unknown-elf``).
   :param COMPIERT_RT: compiler-rt baremetal subdirectory for this ABI
      (e.g. ``rv64imc``).
   :param SOURCES: C and assembly source files compiled into the host ELF.
   :param DEVICE_DEPS: *(optional)* Device ELF targets that must finish
      building before symbol extraction begins.  Used for build ordering only;
      no object code is pulled from these targets.
   :param DEVICE_SYMBOLS: *(optional)* ``${dev}_symbols`` INTERFACE libraries
      produced by :cmake:command:`add_device_binary`.  Their compiled object
      files (the ``.set`` stubs) are linked into the host ELF so that device
      symbols resolve to their correct absolute addresses.
   :param LAST_DEVICE: *(optional)* Name of the device at the tail of the
      placement chain.  Its ``_placement.ldh`` is included by the host linker
      script so host ``.text`` is placed immediately after all device code.
      Omit or leave empty for host-only builds.
   :param PICOLIBC: *(optional)* Picolibc library variant to link
      (e.g. ``rv64imc``).

   Linker script template placeholders substituted by this function:

   - ``@CHIMERA_PLACEMENT_INCLUDE@`` — empty when ``LAST_DEVICE`` is unset,
     or ``INCLUDE <last_device>_placement.ldh`` otherwise
   - ``@CHIMERA_RESERVED_SECTION@`` — empty when ``LAST_DEVICE`` is unset,
     or a NOLOAD ``.reserved`` section advancing the LC past all device memory

   Post-build targets created:

   - ``chimera_check_overlaps`` — memory map printer and VMA conflict checker
     (``scripts/check_section_overlaps.py``)
   - ``chimera_merge_elf`` — *(optional, requires* ``CHIMERA_UNIFIED_ELF=ON`` *)* merges
     device + host ELFs into one mixed-ISA file via ``lief``
   - ``chimera_footer`` — prints the build summary

   .. code-block:: cmake
      :caption: Example Usage

      add_host_binary(${TEST_NAME}
          ISA          ${ISA_HOST}
          ABI          ${ABI_HOST}
          COMPILER     ${CROSS_COMPILE_HOST}
          COMPIERT_RT  ${COMPILERRT_HOST}
          PICOLIBC     ${PICOLIB_HOST}
          TARGET_DIR   ${CHIMERA_TARGET_DIR}
          DEVICE_DIR   ${CHIMERA_TARGET_DIR}/host
          SOURCES      ${TEST_HOST_SRCS}
          DEVICE_DEPS    ${TEST_NAME}_device
          DEVICE_SYMBOLS ${TEST_NAME}_device_symbols
          LAST_DEVICE    ${TEST_NAME}_device
      )

#]=======================================================================]
function(add_host_binary TARGET_NAME)
    set(oneValueArgs   TARGET_DIR DEVICE_DIR LAST_DEVICE ISA ABI COMPILER COMPIERT_RT PICOLIBC)
    set(multiValueArgs SOURCES DEVICE_DEPS DEVICE_SYMBOLS)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Fall back to global host settings when ISA/ABI not given explicitly
    if(NOT ARG_ISA)
        set(ARG_ISA "${ISA_HOST}")
        message(WARNING "ISA not specified for host binary ${TARGET_NAME}, falling back to ${ARG_ISA}")
    endif()
    if(NOT ARG_ABI)
        set(ARG_ABI "${ABI_HOST}")
        message(WARNING "ABI not specified for host binary ${TARGET_NAME}, falling back to ${ARG_ABI}")
    endif()
    if(NOT ARG_COMPILER)
        set(ARG_COMPILER "${CROSS_COMPILE_HOST}")
        message(WARNING "Compiler not specified for host binary ${TARGET_NAME}, falling back to ${ARG_COMPILER}")
    endif()
    if(NOT ARG_COMPIERT_RT)
        set(ARG_COMPIERT_RT "${COMPILERRT_HOST}")
        message(WARNING "Compiler-rt not specified for host binary ${TARGET_NAME}, falling back to ${ARG_COMPIERT_RT}")
    endif()

    message(STATUS "[CHIMERA] Host (${TARGET_NAME})")
    message(STATUS "[CHIMERA]    ISA / ABI    : ${ARG_ISA} / ${ARG_ABI}")
    message(STATUS "[CHIMERA]    Compiler     : ${ARG_COMPILER}")
    message(STATUS "[CHIMERA]    Compiler-rt  : ${ARG_COMPIERT_RT}")
    if(ARG_PICOLIBC)
        message(STATUS "[CHIMERA]    Picolibc     : ${ARG_PICOLIBC}")
    endif()
    message(STATUS "[CHIMERA]    -> placed after: ${ARG_LAST_DEVICE}")
    message(STATUS "[CHIMERA] =============================================================")

    # Derive compiler-rt library suffix from ABI
    if(ARG_ABI MATCHES "^ilp32*")
        set(_compiert_suffix "riscv32")
    elseif(ARG_ABI MATCHES "^lp64")
        set(_compiert_suffix "riscv64")
    else()
        message(FATAL_ERROR "[CHIMERA] Cannot determine compiler-rt suffix from ABI: ${ARG_ABI}")
    endif()

    # -----------------------------------------------------------------------
    # 1. Generate the host linker script from template
    # -----------------------------------------------------------------------
    if(ARG_LAST_DEVICE)
        set(CHIMERA_PLACEMENT_INCLUDE "INCLUDE ${ARG_LAST_DEVICE}_placement.ldh")
        set(CHIMERA_RESERVED_SECTION "")
        string(APPEND CHIMERA_RESERVED_SECTION "  .reserved (NOLOAD) : ALIGN(0x1000) {\n")
        string(APPEND CHIMERA_RESERVED_SECTION "    . = __${ARG_LAST_DEVICE}_end;\n")
        string(APPEND CHIMERA_RESERVED_SECTION "  } > memisl\n")
    else()
        set(CHIMERA_PLACEMENT_INCLUDE "")
        set(CHIMERA_RESERVED_SECTION "")
    endif()
    set(GEN_LINK_LD "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_link.ld")
    configure_file("${ARG_DEVICE_DIR}/link.ld.in" "${GEN_LINK_LD}" @ONLY)
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${GEN_LINK_LD}")

    # -----------------------------------------------------------------------
    # 2. Compile the host ELF
    # -----------------------------------------------------------------------
    add_executable(${TARGET_NAME}_host.elf ${ARG_SOURCES})

    # Place ELF in the subdirectory and suppress the global .elf suffix
    # (the suffix is already part of the target name).
    set_target_properties(${TARGET_NAME}_host.elf PROPERTIES
        SUFFIX ""
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

    target_compile_options(${TARGET_NAME}_host.elf PRIVATE
        --target=${ARG_COMPILER}
        -march=${ARG_ISA}
        -mabi=${ARG_ABI}
        -mcmodel=medany
        -mno-relax
        -O2
        -g
        -Wall
        -Wextra
    )

    target_link_options(${TARGET_NAME}_host.elf PRIVATE
        --target=${ARG_COMPILER}
        -march=${ARG_ISA}
        -mabi=${ARG_ABI}
        -mcmodel=medany
        -nostdlib
        -T${GEN_LINK_LD}
        -Wl,--build-id=none
        -lclang_rt.builtins-${_compiert_suffix}
    )

    target_include_directories(${TARGET_NAME}_host.elf PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
    )

    target_link_directories(${TARGET_NAME}_host.elf PRIVATE
        ${TOOLCHAIN_DIR}/lib/clang/${LLVM_VERSION_MAJOR}/lib/baremetal/${ARG_COMPIERT_RT}
        ${ARG_DEVICE_DIR}
        ${ARG_TARGET_DIR}/shared
        ${CMAKE_CURRENT_BINARY_DIR}   # so the linker can find *_placement.ldh via INCLUDE
    )

    # Relink when the generated linker script or last device's placement changes
    if(ARG_LAST_DEVICE)
        set(LAST_PLACEMENT_LDH "${CMAKE_CURRENT_BINARY_DIR}/${ARG_LAST_DEVICE}_placement.ldh")
        set_target_properties(${TARGET_NAME}_host.elf PROPERTIES
            LINK_DEPENDS "${GEN_LINK_LD};${LAST_PLACEMENT_LDH}"
        )
    else()
        set_target_properties(${TARGET_NAME}_host.elf PROPERTIES
            LINK_DEPENDS "${GEN_LINK_LD}"
        )
    endif()

    # -----------------------------------------------------------------------
    # 3. Device build-order dependencies
    # -----------------------------------------------------------------------
    if(ARG_DEVICE_DEPS)
        add_dependencies(${TARGET_NAME}_host.elf ${ARG_DEVICE_DEPS})
    endif()

    # -----------------------------------------------------------------------
    # 4. Link device symbol objects
    # -----------------------------------------------------------------------
    foreach(dev_lib IN LISTS ARG_DEVICE_SYMBOLS)
        target_link_libraries(${TARGET_NAME}_host.elf PRIVATE ${dev_lib})
    endforeach()

    # -----------------------------------------------------------------------
    # 5. Create and link libraries
    # -----------------------------------------------------------------------
    if(ARG_PICOLIBC)
        set(_picolibc_host_lib "${PICOLIBC_DIR}/lib/${ARG_PICOLIBC}/libc.a")
        set(_picolibc_host_inc "${PICOLIBC_DIR}/include")

        if(NOT EXISTS "${_picolibc_host_lib}")
            message(FATAL_ERROR
                "[CHIMERA] Picolibc host library not found:\n"
                "  ${_picolibc_host_lib}\n"
                "Run:  make picolibc-multilib   (from the chimera-sdk root)\n"
                "Or set PICOLIBC_DIR if picolibc is installed elsewhere.")
        endif()

        add_library(${TARGET_NAME}_picolibc STATIC IMPORTED GLOBAL)
        set_target_properties(${TARGET_NAME}_picolibc  PROPERTIES
            IMPORTED_LOCATION "${_picolibc_host_lib}"
        )
        set_target_properties(${TARGET_NAME}_picolibc  PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${_picolibc_host_inc}"
        )
        target_link_libraries(${TARGET_NAME}_host.elf PRIVATE ${TARGET_NAME}_picolibc)
    endif()

    # Create per-binary host runtime (compiled with this binary's ISA/ABI)
    get_property(_host_runtime_sources  GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_SOURCES)
    get_property(_host_runtime_includes GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_INCLUDES)
    get_property(_host_runtime_priv_inc GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_PRIVATE_INCLUDES)
    get_property(_host_runtime_defines  GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_DEFINES)
    if(_host_runtime_sources)
        add_library(${TARGET_NAME}_host_runtime OBJECT ${_host_runtime_sources})
        target_compile_options(${TARGET_NAME}_host_runtime PRIVATE
            --target=${ARG_COMPILER}
            -march=${ARG_ISA}
            -mabi=${ARG_ABI}
            -mno-relax
            -mstrict-align
            -O2
            -ffunction-sections
            -fdata-sections
        )
        if(_host_runtime_includes)
            target_include_directories(${TARGET_NAME}_host_runtime PUBLIC ${_host_runtime_includes})
        endif()
        if(_host_runtime_priv_inc)
            target_include_directories(${TARGET_NAME}_host_runtime PRIVATE ${_dev_runtime_priv_inc})
        endif()
        if(_host_runtime_defines)
            target_compile_definitions(${TARGET_NAME}_host_runtime PUBLIC ${_host_runtime_defines})
        endif()
        target_link_libraries(${TARGET_NAME}_host_runtime PRIVATE ${TARGET_NAME}_picolibc)
        target_link_libraries(${TARGET_NAME}_host.elf PRIVATE ${TARGET_NAME}_host_runtime)
    endif()

    # -----------------------------------------------------------------------
    # 6. Debug artifacts
    # -----------------------------------------------------------------------
    add_custom_command(
        OUTPUT  ${TARGET_NAME}_host.dump ${TARGET_NAME}_host.sections ${TARGET_NAME}_host.symbols
        COMMAND ${CMAKE_OBJDUMP} -S ${TARGET_NAME}_host.elf > ${TARGET_NAME}_host.dump
        COMMAND ${CMAKE_OBJDUMP} -h ${TARGET_NAME}_host.elf > ${TARGET_NAME}_host.sections
        COMMAND ${CMAKE_NM}      -n ${TARGET_NAME}_host.elf > ${TARGET_NAME}_host.symbols
        DEPENDS ${TARGET_NAME}_host.elf
        COMMENT "[CHIMERA] Disassembling ${TARGET_NAME}"
        VERBATIM
    )

    # -----------------------------------------------------------------------
    # 7. Build per-host binary lists by querying target properties set by
    #    add_device_binary(), then appending this host binary.
    #    This replaces the old global-property accumulation (steps 6 & 7)
    #    so that each add_host_binary() call operates on its own closed set
    #    of binaries rather than everything registered so far.
    # -----------------------------------------------------------------------
    set(_local_names    "")
    set(_local_sections "")
    set(_local_elfs     "")
    set(_local_deps     "")

    foreach(_dev IN LISTS ARG_DEVICE_DEPS)
        get_property(_sf TARGET ${_dev} PROPERTY CHIMERA_SECTIONS_FILE)
        get_property(_ef TARGET ${_dev} PROPERTY CHIMERA_ELF_FILE)
        list(APPEND _local_names    "${_dev}")
        list(APPEND _local_sections "${_sf}")
        list(APPEND _local_elfs     "${_ef}")
        list(APPEND _local_deps     "${_dev}")
    endforeach()

    list(APPEND _local_names    "${TARGET_NAME}")
    list(APPEND _local_sections "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_host.sections")
    list(APPEND _local_elfs     "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_host.elf")
    list(APPEND _local_deps     "${TARGET_NAME}_host.elf")

    # -----------------------------------------------------------------------
    # 8. Write chimera_section_files.cmake (read by PrintBuildFooter).
    #    Each host binary writes its own file in its binary directory so that
    #    the per-host footer target below picks up the correct binary set.
    # -----------------------------------------------------------------------
    set(CHIMERA_LISTS_FILE "${CMAKE_CURRENT_BINARY_DIR}/chimera_section_files.cmake")
    file(WRITE "${CHIMERA_LISTS_FILE}"
        "# Auto-generated by Chimera.cmake — do not edit\n"
        "set(BINARY_NAMES  \"${_local_names}\")\n"
        "set(SECTION_FILES \"${_local_sections}\")\n"
        "set(ELF_FILES     \"${_local_elfs}\")\n"
    )
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${CHIMERA_LISTS_FILE}")

    # -----------------------------------------------------------------------
    # 9. Build --binary <file> argument list for the overlap checker.
    # -----------------------------------------------------------------------
    set(_check_args "")

    # Ensure VERBOSE is not cached; read fresh from environment each time
    set(ENV_VERBOSE $ENV{VERBOSE})

    if(VERBOSE OR ENV_VERBOSE)
        list(APPEND _check_args "--verbose")
    endif()

    foreach(_sf IN LISTS _local_sections)
        list(APPEND _check_args "--binary" "${_sf}")
    endforeach()

    # -----------------------------------------------------------------------
    # 10. chimera_check_overlaps_<host> — per-host Python overlap checker.
    #     Depends only on this host's own binaries (device deps + host).
    #     Aggregated under the convenience target chimera_check_overlaps.
    # -----------------------------------------------------------------------
    set(_check_target "chimera_check_overlaps_${TARGET_NAME}")
    set(_footer_dep "")  # dependencies for the host footer target; start empty and append the check target if UV is found

    if(CHIMERA_UV)
        add_custom_target(${_check_target} ALL
            COMMAND
                "${CHIMERA_UV}" run
                python "${CMAKE_SOURCE_DIR}/scripts/check_section_overlaps.py"
                ${_check_args}
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            DEPENDS
                ${_local_deps}
                ${TARGET_NAME}_host.sections
            COMMENT           "[CHIMERA] Checking memory layout for section overlaps (${TARGET_NAME})"
            VERBATIM
        )
        list(APPEND _footer_dep ${_check_target})
    else()
        message(WARNING "[CHIMERA] UV not found, skipping overlap check for ${TARGET_NAME}")
    endif()

    # -----------------------------------------------------------------------
    # 11. (Optional) chimera_merge_elf_<host> — unified mixed-ISA ELF via lief.
    #     Enabled when CHIMERA_UNIFIED_ELF=ON at configure time.
    #     Created once per host binary.
    # -----------------------------------------------------------------------

    if(CHIMERA_UNIFIED_ELF AND CHIMERA_UV)
        set(UNIFIED_ELF "${TARGET_NAME}_unified")

        # Build --host / --device argument list from local lists
        set(_merge_args "")
        foreach(_dev IN LISTS ARG_DEVICE_DEPS)
            get_property(_ef TARGET ${_dev} PROPERTY CHIMERA_ELF_FILE)
            list(APPEND _merge_args "--device" "${_ef}")
        endforeach()
        list(APPEND _merge_args "--host" "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_host.elf")
        list(APPEND _merge_args "--output" "${CMAKE_CURRENT_BINARY_DIR}/${UNIFIED_ELF}.elf")

        add_custom_command(
            OUTPUT ${UNIFIED_ELF}.elf
            COMMAND
                "${CHIMERA_UV}" run
                python "${CMAKE_SOURCE_DIR}/scripts/merge_elf.py"
                ${_merge_args}
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            DEPENDS           ${_check_target} ${_local_deps} ${TARGET_NAME}_host.sections
            COMMENT           "[CHIMERA] Generating unified mixed-ISA ELF: ${UNIFIED_ELF}.elf"
            VERBATIM
        )

        add_custom_command(
            OUTPUT  ${UNIFIED_ELF}.dump ${UNIFIED_ELF}.sections ${UNIFIED_ELF}.symbols
            COMMAND ${CMAKE_OBJDUMP} -S ${UNIFIED_ELF}.elf > ${UNIFIED_ELF}.dump
            COMMAND ${CMAKE_OBJDUMP} -h ${UNIFIED_ELF}.elf > ${UNIFIED_ELF}.sections
            COMMAND ${CMAKE_NM}      -n ${UNIFIED_ELF}.elf > ${UNIFIED_ELF}.symbols
            DEPENDS ${UNIFIED_ELF}.elf
            COMMENT "[CHIMERA] Disassembling unified ELF: ${UNIFIED_ELF}"
            VERBATIM
        )
        list(APPEND _footer_dep ${UNIFIED_ELF}.elf ${UNIFIED_ELF}.dump ${UNIFIED_ELF}.sections ${UNIFIED_ELF}.symbols)
    elseif(CHIMERA_UNIFIED_ELF AND NOT CHIMERA_UV)
        message(WARNING "[CHIMERA] UV not found, skipping unified ELF generation for ${TARGET_NAME}")
    endif()

    # -----------------------------------------------------------------------
    # 12. chimera_footer_<host> — per-host build summary.
    #     Uses the chimera_section_files.cmake written in step 8 so that only
    #     this host's binaries are printed.
    #     All per-host footer targets are aggregated under chimera_footer.
    # -----------------------------------------------------------------------
    add_custom_target(${TARGET_NAME} ALL
        COMMAND
            ${CMAKE_COMMAND}
            -D "CHIMERA_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}"
            -D "CHIMERA_LISTS_FILE=${CHIMERA_LISTS_FILE}"
            -D "CHIMERA_UNIFIED_ELF=${CHIMERA_UNIFIED_ELF}"
            -D "CHIMERA_UNIFIED_ELF_PATH=${CMAKE_CURRENT_BINARY_DIR}/${UNIFIED_ELF}"
            -P "${CMAKE_SOURCE_DIR}/cmake/scripts/PrintBuildFooter.cmake"
        DEPENDS
            ${_footer_dep}
            ${TARGET_NAME}_host.dump
            ${TARGET_NAME}_host.sections
            ${TARGET_NAME}_host.symbols
        COMMENT           "[CHIMERA] Printing build summary for ${TARGET_NAME}"
        VERBATIM
    )

    # Link binaries all ELF files in the ${CMAKE_CURRENT_BINARY_DIR} to build/bin
    # -----------------------------------------------------------------------
    # 13. Copy all ELF binaries to build/bin for convenient access
    # -----------------------------------------------------------------------
    add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin"
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_host.elf
            "${CMAKE_BINARY_DIR}/bin/${TARGET_NAME}_host.elf"
        COMMENT "[CHIMERA] Copying ${TARGET_NAME}_host.elf to build/bin"
        VERBATIM
    )
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_BINARY_DIR}/bin/${TARGET_NAME}_host.elf")

    if(CHIMERA_UNIFIED_ELF)
        add_custom_command(
            TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin"
            COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/${UNIFIED_ELF}.elf
                "${CMAKE_BINARY_DIR}/bin/${UNIFIED_ELF}.elf"
            COMMENT "[CHIMERA] Copying ${UNIFIED_ELF}.elf to build/bin"
            VERBATIM
        )
        set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_BINARY_DIR}/bin/${UNIFIED_ELF}.elf")
    endif()
endfunction()
