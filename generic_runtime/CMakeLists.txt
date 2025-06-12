# Copyright 2025 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Victor Jung <jungvi@iis.ee.ethz.ch>

file(GLOB_RECURSE C_SOURCES
  "*.c"
)

target_include_directories(runtime_host
  PRIVATE ${PROJECT_SOURCE_DIR}/hal/inc
)

target_sources(runtime_host PRIVATE ${C_SOURCES})
add_dependencies(runtime_host picolibc)