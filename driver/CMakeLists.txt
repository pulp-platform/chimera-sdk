# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Philip Wiese <wiesep@iis.ee.ethz.ch>


# WIESEP: Add all runtime drivers
add_subdirectory(cluster)

# WIESEP: Export this directory as root include directory for the drivers
target_include_directories(runtime PUBLIC ${CMAKE_CURRENT_LIST_DIR})




