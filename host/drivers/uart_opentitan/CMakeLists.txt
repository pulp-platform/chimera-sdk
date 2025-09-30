# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>

# ------------------------------------------------------------------------------
# Set the variables for the OpenTitan UART driver
# ------------------------------------------------------------------------------
set(TOP_TARGET "earlgrey" CACHE STRING "Top-level target for OpenTitan IP generation")
set(OPENTITAN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/opentitan")
set(OPENTITAN_SW_DIR "${OPENTITAN_DIR}/sw")
set(OPENTITAN_TOP_DIR "${OPENTITAN_DIR}/hw/top_${TOP_TARGET}")
set(GENERATE_OT_IPS_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/../../scripts/opentitan/generate_ot_ips.py")
set(GENERATED_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/opentitan/hw/top_${TOP_TARGET}")

# VIVIANEP: The following commit hash has been verified to
#           successfully build the OpenTitan IPs for the UART driver.
set(OPENTITAN_COMMIT_HASH f995b5639fe7eefee047e03ccf262c0179f79ec8 CACHE STRING "OpenTitan commit hash")
message(STATUS "[CHIMERA-SDK] Using OpenTitan commit: ${OPENTITAN_COMMIT_HASH}")

set(OPENTITAN_SPARSE_PATTERNS
  "util/"
  "sw/device/silicon_creator/lib/error.h"
  "sw/device/silicon_creator/lib/drivers/hmac.h"
  "sw/device/silicon_creator/lib/drivers/hmac.c"
  "sw/device/silicon_creator/lib/base/chip.h"
  "sw/device/lib/testing/test_framework/status.h"
  "sw/device/lib/testing/test_framework/ottf_test_config.h"
  "sw/device/lib/testing/test_framework/ottf_main.h"
  "sw/device/lib/testing/test_framework/FreeRTOSConfig.h"
  "sw/device/lib/testing/test_framework/check.h"
  "sw/device/lib/testing/*.h"
  "sw/device/lib/testing/*.c"
  "sw/device/lib/runtime/irq.h"
  "sw/device/lib/runtime/irq.c"
  "sw/device/lib/runtime/ibex.h"
  "sw/device/lib/runtime/ibex.c"
  "sw/device/lib/runtime/hart.h"
  "sw/device/lib/runtime/hart.c"
  "sw/device/lib/dif/dif_usbdev.h"
  "sw/device/lib/dif/dif_usbdev.c"
  "sw/device/lib/dif/dif_uart.h"
  "sw/device/lib/dif/dif_uart.c"
  "sw/device/lib/dif/dif_sysrst_ctrl.h"
  "sw/device/lib/dif/dif_sysrst_ctrl.c"
  "sw/device/lib/dif/dif_sram_ctrl.h"
  "sw/device/lib/dif/dif_sram_ctrl.c"
  "sw/device/lib/dif/dif_spi_host.h"
  "sw/device/lib/dif/dif_spi_host.c"
  "sw/device/lib/dif/dif_spi_device.h"
  "sw/device/lib/dif/dif_spi_device.c"
  "sw/device/lib/dif/dif_sensor_ctrl.h"
  "sw/device/lib/dif/dif_sensor_ctrl.c"
  "sw/device/lib/dif/dif_rv_plic.h"
  "sw/device/lib/dif/dif_rv_plic.c"
  "sw/device/lib/dif/dif_rv_core_ibex.h"
  "sw/device/lib/dif/dif_rv_core_ibex.c"
  "sw/device/lib/dif/dif_rstmgr.h"
  "sw/device/lib/dif/dif_rstmgr.c"
  "sw/device/lib/dif/dif_pwrmgr.h"
  "sw/device/lib/dif/dif_pwrmgr.c"
  "sw/device/lib/dif/dif_pinmux.h"
  "sw/device/lib/dif/dif_pinmux.c"
  "sw/device/lib/dif/dif_otp_ctrl.h"
  "sw/device/lib/dif/dif_otp_ctrl.c"
  "sw/device/lib/dif/dif_otbn.h"
  "sw/device/lib/dif/dif_otbn.c"
  "sw/device/lib/dif/dif_lc_ctrl.h"
  "sw/device/lib/dif/dif_lc_ctrl.c"
  "sw/device/lib/dif/dif_kmac.h"
  "sw/device/lib/dif/dif_kmac.c"
  "sw/device/lib/dif/dif_keymgr.h"
  "sw/device/lib/dif/dif_keymgr.c"
  "sw/device/lib/dif/dif_i2c.h"
  "sw/device/lib/dif/dif_i2c.c"
  "sw/device/lib/dif/dif_hmac.h"
  "sw/device/lib/dif/dif_hmac.c"
  "sw/device/lib/dif/dif_gpio.h"
  "sw/device/lib/dif/dif_gpio.c"
  "sw/device/lib/dif/dif_flash_ctrl.h"
  "sw/device/lib/dif/dif_flash_ctrl.c"
  "sw/device/lib/dif/dif_entropy_src.h"
  "sw/device/lib/dif/dif_entropy_src.c"
  "sw/device/lib/dif/dif_edn.h"
  "sw/device/lib/dif/dif_edn.c"
  "sw/device/lib/dif/dif_csrng.h"
  "sw/device/lib/dif/dif_csrng.c"
  "sw/device/lib/dif/dif_csrng_shared.h"
  "sw/device/lib/dif/dif_csrng_shared.c"
  "sw/device/lib/dif/dif_clkmgr.h"
  "sw/device/lib/dif/dif_clkmgr.c"
  "sw/device/lib/dif/dif_base.h"
  "sw/device/lib/dif/dif_base.c"
  "sw/device/lib/dif/dif_aon_timer.h"
  "sw/device/lib/dif/dif_aon_timer.c"
  "sw/device/lib/dif/dif_alert_handler.h"
  "sw/device/lib/dif/dif_alert_handler.c"
  "sw/device/lib/dif/dif_aes.h"
  "sw/device/lib/dif/dif_aes.c"
  "sw/device/lib/dif/dif_adc_ctrl.h"
  "sw/device/lib/dif/dif_adc_ctrl.c"
  "sw/device/lib/base/stdasm.h"
  "sw/device/lib/base/stdasm.c"
  "sw/device/lib/base/status.h"
  "sw/device/lib/base/status.c"
  "sw/device/lib/base/multibits.h"
  "sw/device/lib/base/multibits.c"
  "sw/device/lib/base/mmio.h"
  "sw/device/lib/base/mmio.c"
  "sw/device/lib/base/memory.h"
  "sw/device/lib/base/memory.c"
  "sw/device/lib/base/math.h"
  "sw/device/lib/base/math.c"
  "sw/device/lib/base/math_builtins.c"
  "sw/device/lib/base/macros.h"
  "sw/device/lib/base/internal/status.h"
  "sw/device/lib/base/internal/absl_status.h"
  "sw/device/lib/base/hardened.h"
  "sw/device/lib/base/hardened.c"
  "sw/device/lib/base/hardened_asm.h"
  "sw/device/lib/base/csr.h"
  "sw/device/lib/base/csr_registers.h"
  "sw/device/lib/base/bitfield.h"
  "sw/device/lib/base/bitfield.c"
  "sw/device/lib/base/adv_macros.h"
  "sw/device/lib/base/abs_mmio.h"
  "sw/device/lib/base/abs_mmio.c"
  "sw/device/lib/arch/device.h"
  "sw/device/lib/arch/device_silicon.c"
  "sw/device/lib/arch/boot_stage.h"
  "hw/top_earlgrey/templates/*.tpl"
  "hw/top_earlgrey/ip"
  "hw/top_earlgrey/ip_autogen/*/data/*.hjson"
  "hw/top_earlgrey/data/otp/*.hjson"
  "hw/top_earlgrey/data/*.hjson"
  "hw/ip/aes/model/aes_modes.h"
  "hw/ip/aes/model/aes_modes.c"
  "hw/ip/*/data/*.hjson"
  "hw/ip_templates/"
  )

# ------------------------------------------------------------------------------
# Call the OpenTitan driver setup
# ------------------------------------------------------------------------------
include(${PROJECT_SOURCE_DIR}/cmake/opentitan.cmake)

# ------------------------------------------------------------------------------
# Prepare for header generation using the OT tools
# ------------------------------------------------------------------------------
find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_custom_target(generate_ot_ips ALL
  COMMAND ${Python3_EXECUTABLE} -u ${GENERATE_OT_IPS_SCRIPT} --output ${GENERATED_OUTPUT_DIR}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}  # Ensure relative paths resolve correctly.
  DEPENDS ${GENERATE_OT_IPS_SCRIPT}
  COMMENT "[CHIMERA-SDK] Generating OpenTitan UART IP for ${TOP_TARGET} into ${GENERATED_OUTPUT_DIR}"
)

# ------------------------------------------------------------------------------
# Add the UART driver library
# ------------------------------------------------------------------------------
add_library(uart_lib STATIC)
add_dependencies(uart_lib generate_ot_ips)

file(GLOB_RECURSE UART_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
  "${OPENTITAN_DIR}/hw/top_${TOP_TARGET}/dt/*.c"
  "${OPENTITAN_DIR}/hw/top_${TOP_TARGET}/sw/autogen/*.c"
  "${OPENTITAN_SW_DIR}/device/lib/arch/*.c"
  "${OPENTITAN_SW_DIR}/device/lib/base/*.c"
  "${OPENTITAN_SW_DIR}/device/lib/dif/*.c"
  "${OPENTITAN_SW_DIR}/device/lib/dif/autogen/*.c"
  "${OPENTITAN_SW_DIR}/device/lib/runtime/*.c"
)

# VIVIANEP: Make sure to check for changes in the source files.
file(GLOB_RECURSE UART_SOURCES_TMP CONFIGURE_DEPENDS ${UART_SOURCES})
list(FILTER UART_SOURCES EXCLUDE REGEX ".*/tests/.*")

if(${TOP_TARGET} STREQUAL "earlgrey")
  target_compile_definitions(uart_lib PUBLIC OPENTITAN_IS_EARLGREY)
elseif(${TOP_TARGET} STREQUAL "darjeeling")
  target_compile_definitions(uart_lib PUBLIC OPENTITAN_IS_DARJEELING)
else()
  message(FATAL_ERROR "[CHIMERA-SDK] Invalid top-level target for OpenTitan IPs: ${TOP_TARGET}")
endif()

# VIVIANEP: We must enable optimization flags to avoid overflowing
#           the memory island of the Chimera SoC.
target_compile_options(uart_lib PRIVATE -Os -flto)
target_link_options(uart_lib PRIVATE -Wl,--gc-sections)

target_sources(
  uart_lib
  PRIVATE
    ${UART_SOURCES}
)

target_include_directories(
  uart_lib
  PUBLIC
  ${OPENTITAN_TOP_DIR}
  ${OPENTITAN_SW_DIR}/device/lib/dif/autogen
  ${OPENTITAN_DIR}
  ${PROJECT_SOURCE_DIR}/host/hal/inc
  ${PROJECT_SOURCE_DIR}/host/drivers/uart_opentitan/inc
)