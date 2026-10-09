# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

# WIESEP: It is important to set the ISA and ABI for the host and the cluster snitch
set(ABI_HOST ilp32)
set(ISA_HOST rv32imc)
set(PICOLIB_HOST rv32im/ilp32)
# Use rv32imc baremetal compiler-rt (matches ISA_HOST; rv32im dir does not exist)
set(COMPILERRT_HOST rv32imc)
set(CROSS_COMPILE_HOST "riscv32-unknown-elf")

set(ABI_CLUSTER_SNITCH ilp32)
set(ISA_CLUSTER_SNITCH rv32ima_xdma)
set(PICOLIB_CLUSTER_SNITCH rv32im/ilp32)
# Use rv32ima baremetal compiler-rt (base ISA of rv32ima_xdma; rv32im dir does not exist)
set(COMPILERRT_CLUSTER_SNITCH rv32ima)
set(CROSS_COMPILE_DEVICE_SNITCH_CLUSTER "riscv32-unknown-elf")

