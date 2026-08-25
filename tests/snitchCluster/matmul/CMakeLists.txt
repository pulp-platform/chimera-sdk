# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set(TEST_NAME test_snitchCluster_matmul)

######## Test Vectors ##########################################################
chimera_test_vector_generator(
  GENERATOR ${CMAKE_CURRENT_SOURCE_DIR}/generate_vectors.py
  OUTPUTS
            ${CMAKE_CURRENT_SOURCE_DIR}/include/testinputs_16x16x16.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/testoutputs_16x16x16.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/testinputs_32x48x64.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/testoutputs_32x48x64.h
)

######## Matrix Shape ##########################################################
# Pick the MxNxP shape to build for, e.g. -DMATMUL_SHAPE=32x48x64
chimera_select_shape(
  OPTION          MATMUL_SHAPE
  DEFAULT         16x16x16
  HEADER_PREFIXES testinputs_ testoutputs_
)

######## Source files ###########################################################
file(GLOB_RECURSE TEST_HOST_SRCS    "src_host/*.c")
file(GLOB_RECURSE TEST_SNITCH_SRCS  "src_cluster/*.c")

######## DEVICE Binary #########################################################
add_device_binary(${TEST_NAME}_device1
    DEVICE_NAME  device1
    ISA          ${ISA_CLUSTER_SNITCH}
    ABI          ${ABI_CLUSTER_SNITCH}
    COMPILER     ${CROSS_COMPILE_DEVICE_SNITCH_CLUSTER}
    COMPIERT_RT  ${COMPILERRT_CLUSTER_SNITCH}
    PICOLIBC     ${PICOLIB_CLUSTER_SNITCH}
    TARGET_DIR   ${CHIMERA_TARGET_DIR}
    DEVICE_DIR   ${CHIMERA_TARGET_DIR}/devices/snitch_cluster
    SOURCES
        ${TEST_SNITCH_SRCS}
)
target_include_directories(${TEST_NAME}_device1.elf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(${TEST_NAME}_device1.elf
    PRIVATE
    TESTINPUTS_HEADER="testinputs_${SELECTED_SHAPE}.h"
    TESTOUTPUTS_HEADER="testoutputs_${SELECTED_SHAPE}.h"
)
target_compile_options(${TEST_NAME}_device1.elf PRIVATE -O2 -ffunction-sections -fdata-sections)
# target_link_options(${TEST_NAME}_device1.elf PRIVATE -Wl,--gc-sections)

# ######## DEVICE Binary #########################################################
# add_device_binary(${TEST_NAME}_device2
#     DEVICE_NAME  device2
#     ISA          ${ISA_CLUSTER_SNITCH}
#     ABI          ${ABI_CLUSTER_SNITCH}
#     COMPILER     ${CROSS_COMPILE_DEVICE_SNITCH_CLUSTER}
#     COMPIERT_RT  ${COMPILERRT_CLUSTER_SNITCH}
#     PICOLIBC     ${PICOLIB_CLUSTER_SNITCH}
#     TARGET_DIR   ${CHIMERA_TARGET_DIR}
#     DEVICE_DIR   ${CHIMERA_TARGET_DIR}/devices/snitch_cluster
#     PREV_DEVICE  ${TEST_NAME}_device1
#     SOURCES
#         ${TEST_SNITCH_SRCS}
# )
# target_include_directories(${TEST_NAME}_device2.elf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
# target_compile_options(${TEST_NAME}_device2.elf PRIVATE -O2 -ffunction-sections -fdata-sections)
# # target_link_options(${TEST_NAME}_device2.elf PRIVATE -Wl,--gc-sections)

######## HOST Binary ###########################################################
add_host_binary(${TEST_NAME}
    ISA          ${ISA_HOST}
    ABI          ${ABI_HOST}
    COMPILER     ${CROSS_COMPILE_HOST}
    COMPIERT_RT  ${COMPILERRT_HOST}
    PICOLIBC     ${PICOLIB_HOST}
    TARGET_DIR   ${CHIMERA_TARGET_DIR}
    DEVICE_DIR   ${CHIMERA_TARGET_DIR}/host
    SOURCES
        ${TEST_HOST_SRCS}
    DEVICE_DEPS    ${TEST_NAME}_device1 #${TEST_NAME}_device2
    DEVICE_SYMBOLS ${TEST_NAME}_device1_symbols #${TEST_NAME}_device2_symbols
    LAST_DEVICE    ${TEST_NAME}_device1 #${TEST_NAME}_device2
)

target_include_directories(${TEST_NAME}_host.elf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
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
