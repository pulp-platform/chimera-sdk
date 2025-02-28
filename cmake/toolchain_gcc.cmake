# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/riscv32-unknown-elf-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/riscv32-unknown-elf-g++)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_OBJCOPY ${TOOLCHAIN_DIR}/bin/riscv32-unknown-elf-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_DIR}/bin/riscv32-unknown-elf-objdump)
set(CMAKE_AR ${TOOLCHAIN_DIR}/bin/riscv32-unknown-elf-ar)
