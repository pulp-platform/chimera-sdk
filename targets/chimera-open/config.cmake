# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

# WIESEP: It is important to set the ISA and ABI for the host and the cluster snitch
# set(ABI_HOST ilp32)
# set(ISA_HOST rv32imc)
# set(PICOLIB_HOST rv32im/ilp32)
# set(COMPILERRT_HOST rv32imc)
# set(CROSS_COMPILE_HOST "riscv32-unknown-elf")

set(ABI_HOST lp64d)
set(ISA_HOST rv64imafdc)
set(PICOLIB_HOST rv64imafdc/lp64d)
set(COMPILERRT_HOST rv64imafdc)
set(CROSS_COMPILE_HOST "riscv64-unknown-elf")

set(ABI_CLUSTER_SNITCH ilp32d)
set(ISA_CLUSTER_SNITCH rv32imafd_xdma)
set(PICOLIB_CLUSTER_SNITCH rv32imafd/ilp32d)
set(COMPILERRT_CLUSTER_SNITCH rv32imafd)
set(CROSS_COMPILE_DEVICE_SNITCH_CLUSTER "riscv32-unknown-elf")
