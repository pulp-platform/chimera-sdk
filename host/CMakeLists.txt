# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Philip Wiese <wiesep@iis.ee.ethz.ch>

add_subdirectory(hal)
add_subdirectory(drivers)
add_subdirectory(runtime)

target_link_libraries(runtime_host
  PUBLIC
    picolibc
)