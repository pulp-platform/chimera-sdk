# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Philip Wiese <wiesep@iis.ee.ethz.ch>

# Define mappings for drivers
set(THIRDPARTY_MAPPINGS
    chimera-convolve:opentitan
    chimera-open:opentitan
    chimera-host:opentitan
)

# Call the macro
add_chimera_subdirectories(${TARGET_PLATFORM} "Third Party" "${THIRDPARTY_MAPPINGS}")
