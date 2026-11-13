# SPDX-FileCopyrightText: 2026 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set(TEST_NAME test_deviceTypes)

######## Second device type ####################################################
# This target's Snitch glue without the snitch-sdk runtime
chimera_add_device_runtime(test_bare
    SOURCES
        ${CHIMERA_TARGET_DIR}/devices/snitch_cluster/src/trampoline.c
        ${CHIMERA_TARGET_DIR}/devices/snitch_cluster/src/symbols.c
        ${CHIMERA_TARGET_DIR}/devices/snitch_cluster/src/crt0.S
    INCLUDES ${CHIMERA_TARGET_DIR}/devices/snitch_cluster/inc
    DEFINES  CHIMERA_DEVICE_TEST_BARE
)

######## Source files ###########################################################
file(GLOB_RECURSE TEST_HOST_SRCS   "src_host/*.c")
file(GLOB_RECURSE TEST_COMMON_SRCS "src_common/*.c")
file(GLOB_RECURSE TEST_SNITCH_SRCS "src_snitch/*.c")
file(GLOB_RECURSE TEST_BARE_SRCS   "src_bare/*.c")

######## DEVICE Binaries #######################################################
add_device_binary(${TEST_NAME}_snitch
    DEVICE_NAME  snitch
    ISA          ${ISA_CLUSTER_SNITCH}
    ABI          ${ABI_CLUSTER_SNITCH}
    COMPILER     ${CROSS_COMPILE_DEVICE_SNITCH_CLUSTER}
    COMPIERT_RT  ${COMPILERRT_CLUSTER_SNITCH}
    PICOLIBC     ${PICOLIB_CLUSTER_SNITCH}
    TARGET_DIR   ${CHIMERA_TARGET_DIR}
    DEVICE_DIR   ${CHIMERA_TARGET_DIR}/devices/snitch_cluster
    SOURCES      ${TEST_COMMON_SRCS} ${TEST_SNITCH_SRCS}
)

add_device_binary(${TEST_NAME}_bare
    DEVICE_NAME  bare
    DEVICE_TYPE  test_bare
    ISA          ${ISA_CLUSTER_SNITCH}
    ABI          ${ABI_CLUSTER_SNITCH}
    COMPILER     ${CROSS_COMPILE_DEVICE_SNITCH_CLUSTER}
    COMPIERT_RT  ${COMPILERRT_CLUSTER_SNITCH}
    PICOLIBC     ${PICOLIB_CLUSTER_SNITCH}
    TARGET_DIR   ${CHIMERA_TARGET_DIR}
    DEVICE_DIR   ${CHIMERA_TARGET_DIR}/devices/snitch_cluster
    PREV_DEVICE  ${TEST_NAME}_snitch
    SOURCES      ${TEST_COMMON_SRCS} ${TEST_BARE_SRCS}
)

foreach(_device snitch bare)
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
    DEVICE_DEPS    ${TEST_NAME}_snitch ${TEST_NAME}_bare
    DEVICE_SYMBOLS ${TEST_NAME}_snitch_symbols ${TEST_NAME}_bare_symbols
    LAST_DEVICE    ${TEST_NAME}_bare
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
