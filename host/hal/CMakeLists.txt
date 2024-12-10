# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

file(GLOB_RECURSE SOURCES
  "src/*"
)

add_library(hal_host STATIC ${SOURCES})

target_include_directories(hal_host
  PUBLIC
  "inc/"
)

target_link_libraries(hal_host
  PUBLIC
  picolibc_host
)

if (${DISASSEMBLE_LIBRARIES})
  disassemble_target(hal_host)
endif()
