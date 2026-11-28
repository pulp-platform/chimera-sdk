# SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set(TEST_NAME test_offloadHandshake)

######## Second device type ####################################################
# This target's Snitch glue without the snitch-sdk runtime: two runtimes do not fit in chimera-open
chimera_add_device_runtime(handshake_bare
    SOURCES
        ${CHIMERA_TARGET_DIR}/devices/snitch_cluster/src/trampoline.c
        ${CHIMERA_TARGET_DIR}/devices/snitch_cluster/src/symbols.c
        ${CHIMERA_TARGET_DIR}/devices/snitch_cluster/src/crt0.S
    INCLUDES ${CHIMERA_TARGET_DIR}/devices/snitch_cluster/inc
)

######## Source files ###########################################################
file(GLOB_RECURSE TEST_HOST_SRCS   "src_host/*.c")
file(GLOB_RECURSE TEST_DEVICE_SRCS "src_device/*.c")

######## DEVICE Binaries #######################################################
# Two binaries of two types, so the two offloads of a round jump to different trampolines
add_device_binary(${TEST_NAME}_a
    DEVICE_NAME  a
    ISA          ${ISA_CLUSTER_SNITCH}
    ABI          ${ABI_CLUSTER_SNITCH}
    COMPILER     ${CROSS_COMPILE_DEVICE_SNITCH_CLUSTER}
    COMPIERT_RT  ${COMPILERRT_CLUSTER_SNITCH}
    PICOLIBC     ${PICOLIB_CLUSTER_SNITCH}
    TARGET_DIR   ${CHIMERA_TARGET_DIR}
    DEVICE_DIR   ${CHIMERA_TARGET_DIR}/devices/snitch_cluster
    SOURCES      ${TEST_DEVICE_SRCS}
)

add_device_binary(${TEST_NAME}_b
    DEVICE_NAME  b
    DEVICE_TYPE  handshake_bare
    ISA          ${ISA_CLUSTER_SNITCH}
    ABI          ${ABI_CLUSTER_SNITCH}
    COMPILER     ${CROSS_COMPILE_DEVICE_SNITCH_CLUSTER}
    COMPIERT_RT  ${COMPILERRT_CLUSTER_SNITCH}
    PICOLIBC     ${PICOLIB_CLUSTER_SNITCH}
    TARGET_DIR   ${CHIMERA_TARGET_DIR}
    DEVICE_DIR   ${CHIMERA_TARGET_DIR}/devices/snitch_cluster
    PREV_DEVICE  ${TEST_NAME}_a
    SOURCES      ${TEST_DEVICE_SRCS}
)

target_compile_definitions(${TEST_NAME}_a.elf PRIVATE DEVICE_TAG=0xA)
target_compile_definitions(${TEST_NAME}_b.elf PRIVATE DEVICE_TAG=0xB)

foreach(_device a b)
    target_include_directories(${TEST_NAME}_${_device}.elf PRIVATE include)
    target_compile_options(${TEST_NAME}_${_device}.elf PRIVATE -O2 -ffunction-sections -fdata-sections)
endforeach()

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
    DEVICE_DEPS    ${TEST_NAME}_a ${TEST_NAME}_b
    DEVICE_SYMBOLS ${TEST_NAME}_a_symbols ${TEST_NAME}_b_symbols
    LAST_DEVICE    ${TEST_NAME}_b
)

target_include_directories(${TEST_NAME}_host.elf PRIVATE include)
target_compile_options(${TEST_NAME}_host.elf PRIVATE -O2 -ffunction-sections -fdata-sections)

######## Simulation ############################################################
if(TEST_MODE STREQUAL "simulation")
    add_test(NAME ${TEST_NAME}
        COMMAND ${SIMULATION_BINARY}
            +BINARY=${CMAKE_CURRENT_BINARY_DIR}/chimera_unified_${TEST_NAME}.elf
            +PRELMODE=${PRELOAD_MODE_INT}
    )
endif()
