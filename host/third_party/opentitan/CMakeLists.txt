# Copyright 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set(OPENTITAN_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/opentitan_peripherals)

# Collect all source files
file(GLOB_RECURSE C_SOURCES
    "${OPENTITAN_BASE_DIR}/sw/device/lib/base/bitfield.c"
    # "${OPENTITAN_BASE_DIR}/sw/device/lib/base/memory.c"
    # "${OPENTITAN_BASE_DIR}/sw/device/lib/base/mmio.c"
    "${OPENTITAN_BASE_DIR}/sw/device/lib/base/internal/*.c"
    "${OPENTITAN_BASE_DIR}/sw/device/lib/dif/dif_gpio.c"
    "${OPENTITAN_BASE_DIR}/sw/device/lib/dif/dif_i2c.c"
    "${OPENTITAN_BASE_DIR}/sw/device/lib/dif/dif_spi_host.c"
    "${OPENTITAN_BASE_DIR}/sw/device/lib/dif/autogen/dif_gpio_autogen.c"
    "${OPENTITAN_BASE_DIR}/sw/device/lib/dif/autogen/dif_i2c_autogen.c"
    "${OPENTITAN_BASE_DIR}/sw/device/lib/dif/autogen/dif_spi_host_autogen.c"
)

# Include directories for consumers
# (add as PUBLIC so consumers inherit them)
target_include_directories(runtime_host PUBLIC
    ${OPENTITAN_BASE_DIR}
)

target_sources(runtime_host PRIVATE ${C_SOURCES})
target_compile_definitions(runtime_host PUBLIC CHIMERA_DRIVER_OPENTITAN)
