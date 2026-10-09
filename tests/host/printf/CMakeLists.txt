# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set(TEST_NAME test_host_printf)

######## Source files ###########################################################
file(GLOB_RECURSE TEST_HOST_SRCS "src_host/*.c")

######## HOST Binary ###########################################################
add_host_binary(${TEST_NAME}
    ISA          ${ISA_HOST}
    ABI          ${ABI_HOST}
    COMPILER     ${CROSS_COMPILE_HOST}
    COMPIERT_RT  ${COMPILERRT_HOST}
    PICOLIBC     ${PICOLIB_HOST}
    TARGET_DIR   ${CHIMERA_TARGET_DIR}
    DEVICE_DIR   ${CHIMERA_TARGET_DIR}/host
    SOURCES      ${TEST_HOST_SRCS}
)
target_compile_options(${TEST_NAME}_host.elf PRIVATE -O2 -ffunction-sections -fdata-sections)
target_link_options(${TEST_NAME}_host.elf PRIVATE -Wl,--gc-sections)

######## Simulation ############################################################
if(TEST_MODE STREQUAL "simulation")
    add_test(NAME ${TEST_NAME}
        COMMAND ${SIMULATION_BINARY}
            +BINARY=${CMAKE_CURRENT_BINARY_DIR}/chimera_unified_${TEST_NAME}.elf
            +PRELMODE=${PRELOAD_MODE_INT}
    )
endif()
