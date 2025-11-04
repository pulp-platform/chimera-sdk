# Copyright 2025 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

# WIESEP: It is important to set the ISA and ABI for the host and the cluster snitch
set(ABI_HOST ilp32)
set(ISA_HOST rv32imc)
set(PICOLIB_HOST rv32im/ilp32)
# WIESEP: To avoid compatibility issues, link against RV32IM libraries
set(COMPILERRT_HOST rv32im)

set(ABI_CLUSTER_SNITCH ilp32)
set(ISA_CLUSTER_SNITCH rv32ima_xdma)
set(PICOLIB_CLUSTER_SNITCH rv32im/ilp32)
# WIESEP: To avoid compatibility issues, link against RV32IM libraries
set(COMPILERRT_CLUSTER_SNITCH rv32im)

