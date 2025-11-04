# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

# Enable verbose output for the linker
# This passes --verbose to the linker (ld), showing detailed linking information
add_link_options("-Wl,--verbose")

# Enable verbose output for the compiler
# The -v flag makes the compiler print the commands it executes during compilation
add_compile_options("-v")

# Enable compiler warnings
add_compile_options(
    # -Wall: Enable most common warnings (e.g., unused variables, uninitialized variables)
    "-Wall"
    # -Wextra: Enable additional warnings beyond -Wall for stricter code quality checks
    "-Wextra"
)

# Enable verbose output for the makefile
# (or run `cmake --build . --verbose=1` / set VERBOSE=1)
# set(CMAKE_VERBOSE_MAKEFILE TRUE)

