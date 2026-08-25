# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set(TEST_NAME test_snitchCluster_hyperbus)

######## Source files ###########################################################
file(GLOB_RECURSE TEST_HOST_SRCS    "src_host/*.c")
file(GLOB_RECURSE TEST_SNITCH_SRCS  "src_cluster/*.c")

######## DEVICE Binary #########################################################
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
target_include_directories(${TEST_NAME}_device.elf PRIVATE include)
target_compile_options(${TEST_NAME}_device.elf PRIVATE -O2 -ffunction-sections -fdata-sections)

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
    DEVICE_DEPS    ${TEST_NAME}_device
    DEVICE_SYMBOLS ${TEST_NAME}_device_symbols
    LAST_DEVICE    ${TEST_NAME}_device
)
target_include_directories(${TEST_NAME}_host.elf PRIVATE include)
target_compile_options(${TEST_NAME}_host.elf PRIVATE -O2 -ffunction-sections -fdata-sections)
# target_link_options(${TEST_NAME}_host.elf PRIVATE -Wl,--gc-sections)

######## Simulation ############################################################
if(TEST_MODE STREQUAL "simulation")
    add_test(NAME ${TEST_NAME}
        COMMAND ${SIMULATION_BINARY}
            +BINARY=${CMAKE_CURRENT_BINARY_DIR}/chimera_unified_${TEST_NAME}.elf
            +PRELMODE=${PRELOAD_MODE_INT}
    )
endif()
