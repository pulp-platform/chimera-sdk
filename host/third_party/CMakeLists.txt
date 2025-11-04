# Copyright 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

# Define mappings for drivers
set(THIRDPARTY_MAPPINGS
    chimera-convolve:opentitan
    chimera-open:opentitan
    chimera-host:opentitan
)

# Call the macro
add_chimera_subdirectories(${TARGET_PLATFORM} "Third Party" "${THIRDPARTY_MAPPINGS}")
