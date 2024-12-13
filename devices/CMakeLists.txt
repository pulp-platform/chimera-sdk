# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Philip Wiese <wiesep@iis.ee.ethz.ch>

# Define mappings for devices
set(DEVICE_MAPPINGS
    chimera-convolve:snitch_cluster
    chimera-open:snitch_cluster
    chimera-host:
)

# Call the macro
add_chimera_subdirectories(${TARGET_PLATFORM} "Device" ${DEVICE_MAPPINGS})
