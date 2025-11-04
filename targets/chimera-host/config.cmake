# Copyright 2025 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

# WIESEP: It is important to set the ISA and ABI for the host and the cluster snitch
set(ABI_HOST ilp32)
set(ISA_HOST rv32imc)
set(PICOLIB_HOST rv32im/ilp32)
set(COMPILERRT_HOST rv32imc)

set(ABI_CLUSTER_SNITCH None)
set(ISA_CLUSTER_SNITCH None)
set(PICOLIB_CLUSTER_SNITCH None)
set(COMPILERRT_CLUSTER_SNITCH None)

