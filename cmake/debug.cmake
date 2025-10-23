# Enable verbose output for the linker
add_link_options("-Wl,--verbose")

# Enable verbose output for the compiler
add_compile_options("-v")

add_compile_options(
    "-Wall"
    "-Wextra"
)

# Enable verbose output for the makefile
# set(CMAKE_VERBOSE_MAKEFILE TRUE)

