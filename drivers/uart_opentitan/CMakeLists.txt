# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Viviane Potocnik <vivianep@iis.ee.ethz.ch>

# ------------------------------------------------------------------------------
# Set the variables for the OpenTitan UART driver
# ------------------------------------------------------------------------------
set(OPENTITAN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/opentitan")
set(OPENTITAN_SW_DIR "${OPENTITAN_DIR}/sw")

set(OPENTITAN_SPARSE_PATTERNS
  "util/"
  "hw/ip_templates/"
  "hw/ip/aes/model/aes_modes.c"
  "hw/ip/aes/model/aes_modes.h"
  "hw/ip/*/data/*.hjson"
  "hw/top_earlgrey/ip_autogen/*/data/*.hjson"
  "hw/top_earlgrey/data/*.hjson"
  "hw/top_earlgrey/data/otp/*.hjson"
  "hw/top_earlgrey/ip"
  "hw/top_earlgrey/templates/*.tpl"
  "sw/device/lib/arch/boot_stage.h"
  "sw/device/lib/arch/device.h"
  "sw/device/lib/arch/device_silicon.c"
  "sw/device/lib/base/abs_mmio.c"
  "sw/device/lib/base/abs_mmio.h"
  "sw/device/lib/base/adv_macros.h"
  "sw/device/lib/base/bitfield.c"
  "sw/device/lib/base/bitfield.h"
  "sw/device/lib/base/csr.h"
  "sw/device/lib/base/csr_registers.h"
  "sw/device/lib/base/hardened.c"
  "sw/device/lib/base/hardened.h"
  "sw/device/lib/base/hardened_asm.h"
  "sw/device/lib/base/macros.h"
  "sw/device/lib/base/math.c"
  "sw/device/lib/base/math.h"
  "sw/device/lib/base/math_builtins.c"
  "sw/device/lib/base/memory.c"
  "sw/device/lib/base/memory.h"
  "sw/device/lib/base/mmio.c"
  "sw/device/lib/base/mmio.h"
  "sw/device/lib/base/multibits.c"
  "sw/device/lib/base/multibits.h"
  "sw/device/lib/base/status.c"
  "sw/device/lib/base/status.h"
  "sw/device/lib/base/stdasm.c"
  "sw/device/lib/base/stdasm.h"
  "sw/device/lib/base/internal/status.h"
  "sw/device/lib/base/internal/absl_status.h"
  "sw/device/lib/dif/dif_aes.c"
  "sw/device/lib/dif/dif_aes.h"
  "sw/device/lib/dif/dif_adc_ctrl.c"
  "sw/device/lib/dif/dif_adc_ctrl.h"
  "sw/device/lib/dif/dif_alert_handler.c"
  "sw/device/lib/dif/dif_alert_handler.h"
  "sw/device/lib/dif/dif_aon_timer.c"
  "sw/device/lib/dif/dif_aon_timer.h"
  "sw/device/lib/dif/dif_base.c"
  "sw/device/lib/dif/dif_base.h"
  "sw/device/lib/dif/dif_clkmgr.c"
  "sw/device/lib/dif/dif_clkmgr.h"
  "sw/device/lib/dif/dif_csrng.c"
  "sw/device/lib/dif/dif_csrng.h"
  "sw/device/lib/dif/dif_csrng_shared.c"
  "sw/device/lib/dif/dif_csrng_shared.h"
  # "sw/device/lib/dif/dif_dma.c"
  # "sw/device/lib/dif/dif_dma.h"
  "sw/device/lib/dif/dif_edn.c"
  "sw/device/lib/dif/dif_edn.h"
  "sw/device/lib/dif/dif_entropy_src.c"
  "sw/device/lib/dif/dif_entropy_src.h"
  "sw/device/lib/dif/dif_flash_ctrl.c"
  "sw/device/lib/dif/dif_flash_ctrl.h"
  "sw/device/lib/dif/dif_gpio.c"
  "sw/device/lib/dif/dif_gpio.h"
  "sw/device/lib/dif/dif_hmac.c"
  "sw/device/lib/dif/dif_hmac.h"
  "sw/device/lib/dif/dif_i2c.c"
  "sw/device/lib/dif/dif_i2c.h"
  "sw/device/lib/dif/dif_keymgr.c"
  "sw/device/lib/dif/dif_keymgr.h"
  # "sw/device/lib/dif/dif_keymgr_dpe.c"
  # "sw/device/lib/dif/dif_keymgr_dpe.h"
  "sw/device/lib/dif/dif_kmac.c"
  "sw/device/lib/dif/dif_kmac.h"
  "sw/device/lib/dif/dif_lc_ctrl.c"
  "sw/device/lib/dif/dif_lc_ctrl.h"
  "sw/device/lib/dif/dif_otbn.c"
  "sw/device/lib/dif/dif_otbn.h"
  "sw/device/lib/dif/dif_otp_ctrl.c"
  "sw/device/lib/dif/dif_otp_ctrl.h"
  "sw/device/lib/dif/dif_pinmux.c"
  "sw/device/lib/dif/dif_pinmux.h"
  "sw/device/lib/dif/dif_pwrmgr.c"
  "sw/device/lib/dif/dif_pwrmgr.h"
  "sw/device/lib/dif/dif_rstmgr.c"
  "sw/device/lib/dif/dif_rstmgr.h"
  "sw/device/lib/dif/dif_rv_core_ibex.c"
  "sw/device/lib/dif/dif_rv_core_ibex.h"
  "sw/device/lib/dif/dif_rv_plic.c"
  "sw/device/lib/dif/dif_rv_plic.h"
  "sw/device/lib/dif/dif_sensor_ctrl.c"
  "sw/device/lib/dif/dif_sensor_ctrl.h"
  "sw/device/lib/dif/dif_spi_host.c"
  "sw/device/lib/dif/dif_spi_host.h"
  "sw/device/lib/dif/dif_spi_device.c"
  "sw/device/lib/dif/dif_spi_device.h"
  "sw/device/lib/dif/dif_sram_ctrl.c"
  "sw/device/lib/dif/dif_sram_ctrl.h"
  "sw/device/lib/dif/dif_sysrst_ctrl.c"
  "sw/device/lib/dif/dif_sysrst_ctrl.h"
  "sw/device/lib/dif/dif_uart.c"
  "sw/device/lib/dif/dif_uart.h"
  "sw/device/lib/dif/dif_usbdev.c"
  "sw/device/lib/dif/dif_usbdev.h"
  "sw/device/lib/runtime/hart.c"
  "sw/device/lib/runtime/hart.h"
  "sw/device/lib/runtime/ibex.c"
  "sw/device/lib/runtime/ibex.h"
  "sw/device/lib/runtime/irq.c"
  "sw/device/lib/runtime/irq.h"
  # "sw/device/lib/runtime/log.c"
  # "sw/device/lib/runtime/log.h"
  # "sw/device/lib/runtime/print.h"
  "sw/device/lib/testing/*.c"
  "sw/device/lib/testing/*.h"
  # "sw/device/lib/testing/json/spi_passthru.c"
  # "sw/device/lib/testing/json/spi_passthru.h"
  "sw/device/lib/testing/test_framework/FreeRTOSConfig.h"
  "sw/device/lib/testing/test_framework/check.h"
  "sw/device/lib/testing/test_framework/status.h"
  "sw/device/lib/testing/test_framework/ottf_test_config.h"
  "sw/device/lib/testing/test_framework/ottf_main.h"
  # "sw/device/lib/ujson/ujson.c"
  # "sw/device/lib/ujson/ujson.h"
  # "sw/device/lib/ujson/ujson_derive.h"
  "sw/device/silicon_creator/lib/base/chip.h"
  # "sw/device/silicon_creator/lib/boot_data.h"
  # "sw/device/silicon_creator/lib/boot_log.h"
  # "sw/device/silicon_creator/lib/chip_info.h"
  "sw/device/silicon_creator/lib/error.h"
  # "sw/device/silicon_creator/lib/nonce.c"
  # "sw/device/silicon_creator/lib/nonce.h"
  "sw/device/silicon_creator/lib/drivers/hmac.c"
  "sw/device/silicon_creator/lib/drivers/hmac.h"
  # "sw/device/silicon_creator/lib/drivers/lifecycle.c"
  # "sw/device/silicon_creator/lib/drivers/lifecycle.h"
  # "sw/device/silicon_creator/lib/drivers/retention_sram.h"
  # "sw/device/silicon_creator/lib/ownership/datatypes.h"
  # "sw/device/silicon_creator/lib/sigverify/ecdsa_p256_key.h"
  # "sw/device/silicon_creator/lib/sigverify/rsa_key.h"
  # "sw/device/silicon_creator/lib/sigverify/spx_key.h"
  # "sw/device/silicon_creator/lib/sigverify/sphincsplus/params.h"
  # "sw/device/silicon_creator/lib/boot_svc/*.h"
  )
  
  # ------------------------------------------------------------------------------
  # Call the OpenTitan driver setup
  # ------------------------------------------------------------------------------
include(${CMAKE_SOURCE_DIR}/cmake/opentitan.cmake)

# ------------------------------------------------------------------------------
# Prepare for header generation using the OT tools
# ------------------------------------------------------------------------------
# Option for the top-level target (default is earlgrey)
set(TOP_TARGET "earlgrey" CACHE STRING "Top-level target for OpenTitan generation")
set(GENERATE_UART_IP_SCRIPT "${CMAKE_SOURCE_DIR}/scripts/opentitan/generate_uart_ip.py")
set(GENERATED_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/opentitan/hw/top_${TOP_TARGET}")

# Create a custom target to run the Python script.
add_custom_target(generate_uart_ip
  COMMAND ${Python3_EXECUTABLE} -u ${GENERATE_UART_IP_SCRIPT} ${OPENTITAN_DIR} ${TOP_TARGET}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}  # Ensure relative paths resolve correctly.
  DEPENDS ${GENERATE_UART_IP_SCRIPT}
  COMMENT "Generating OpenTitan UART IP for ${TOP_TARGET} into ${GENERATED_OUTPUT_DIR}"
)

# Add the dependency to the OpenTitan interface target (created in opentitan.cmake)
add_dependencies(opentitan_interface_${OPENTITAN_COMMIT_HASH} generate_uart_ip)

# ------------------------------------------------------------------------------
# Add the UART driver library
# ------------------------------------------------------------------------------
add_library(uart_lib STATIC)

file(GLOB_RECURSE UART_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
  "${OPENTITAN_SW_DIR}/device/lib/arch/*.c"
  "${OPENTITAN_SW_DIR}/device/lib/base/*.c"
  "${OPENTITAN_SW_DIR}/device/lib/dif/*.c"
  "${OPENTITAN_SW_DIR}/device/lib/runtime/*.c"
  "${OPENTITAN_SW_DIR}/device/lib/dif/autogen/*.c"
  "${OPENTITAN_DIR}/hw/top_${TOP_TARGET}/sw/autogen/*.c"
  "${OPENTITAN_DIR}/hw/top_${TOP_TARGET}/dt/*.c"
)

list(FILTER UART_SOURCES EXCLUDE REGEX ".*/tests/.*")

if(${TOP_TARGET} STREQUAL "earlgrey")
  target_compile_definitions(uart_lib PUBLIC OPENTITAN_IS_EARLGREY)
elseif(${TOP_TARGET} STREQUAL "darjeeling")
  target_compile_definitions(uart_lib PUBLIC OPENTITAN_IS_DARJEELING)
endif()

message(STATUS "USING TOP TARGET: ${TOP_TARGET}")

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
  ${CMAKE_SOURCE_DIR}/hal/inc
  ${CMAKE_SOURCE_DIR}/drivers/uart_opentitan/inc
  ${OPENTITAN_DIR}
  ${OPENTITAN_DIR}/hw/top_${TOP_TARGET}
  ${OPENTITAN_SW_DIR}/device/lib/dif/autogen
)