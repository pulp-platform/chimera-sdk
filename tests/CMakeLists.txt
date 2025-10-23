# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>
# Philip Wiese <wiesep@iis.ee.ethz.ch>

# Define mappings for tests
set(TEST_MAPPINGS
[[
chimera-convolve:
  chimera-convolve,
  host/alloc,
  host/picolibc,
  host/printf,
  host/returnZero,
  host/uartSimple,
  snitchCluster/matmul,
  snitchCluster/snrt,
  snitchCluster/simpleOffload
]]
[[
chimera-open:
  chimera-open,
  host/alloc,
  host/picolibc,
  host/printf,
  host/returnZero,
  host/uartSimple,
  snitchCluster/matmul,
  snitchCluster/snrt,
  snitchCluster/simpleOffload
]]
[[
chimera-host:
  chimera-host,
  host/alloc,
  host/picolibc,
  host/printf,
  host/returnZero,
  host/uartSimple
]]
)

# Call the macro
add_chimera_subdirectories(${TARGET_PLATFORM} "Test" "${TEST_MAPPINGS}")
