# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>

add_subdirectory(generic)

# Check if folder has a CMakeLists.txt file
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${TARGET_PLATFORM}/CMakeLists.txt)
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/${TARGET_PLATFORM})
endif()
