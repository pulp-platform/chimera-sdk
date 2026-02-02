# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set(CMAKE_SYSTEM_NAME Generic)

set(LLVM_TAG llvm)

set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/clang)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/clang++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_DIR}/bin/clang)

set(CMAKE_OBJCOPY ${TOOLCHAIN_DIR}/bin/${LLVM_TAG}-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_DIR}/bin/${LLVM_TAG}-objdump)
set(CMAKE_AR ${TOOLCHAIN_DIR}/bin/${LLVM_TAG}-ar)
set(CMAKE_STRIP ${TOOLCHAIN_DIR}/bin/${LLVM_TAG}-strip)

# Disable ABI detection
set(CMAKE_C_ABI_COMPILED "False")

# Enable WHOLE_ARCHIVE feature
set(CMAKE_LINK_LIBRARY_USING_WHOLE_ARCHIVE
    "-Wl,--whole-archive <LIBRARY> -Wl,--no-whole-archive"
)
set(CMAKE_LINK_LIBRARY_USING_WHOLE_ARCHIVE_SUPPORTED True)


# Use LLVM LLD linker
add_link_options("-fuse-ld=lld")

# Check LLVM version
execute_process(
    COMMAND ${CMAKE_C_COMPILER} --version
    OUTPUT_VARIABLE LLVM_VERSION
)
string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" LLVM_VERSION ${LLVM_VERSION})

# Get Major version
string(REGEX MATCH "^[0-9]+" LLVM_VERSION_MAJOR ${LLVM_VERSION})
string(REGEX MATCH "[0-9]+$" LLVM_VERSION_MINOR ${LLVM_VERSION})
string(REGEX MATCH "[0-9]+$" LLVM_VERSION_PATCH ${LLVM_VERSION})

if(LLVM_VERSION_MAJOR LESS 16)
    message(STATUS "[CHIMERA-SDK] Disable linker relaxation for LLVM < 16")
    set(CMAKE_ALT_C_OPTIONS "-mno-relax")
    set(CMAKE_ALT_LINK_OPTIONS "-Wl,--no-relax")
    # WIESEP: Disable linker relaxation for LLVM 12
    add_compile_options("${CMAKE_ALT_C_OPTIONS}")
    add_link_options("${CMAKE_ALT_LINK_OPTIONS}")
    add_link_options("-Wno-unused-command-line-argument")
endif()

# Define global flags
set(CROSS_COMPILE_HOST "riscv32-unknown-elf")
add_compile_options("--target=${CROSS_COMPILE_HOST}")

add_compile_options(-ggdb -gdwarf-4 -gstrict-dwarf)

message(STATUS "[CHIMERA-SDK] Linking compiler-rt builtins for RV32HOST")
# Prefer compiler-rt rather than libgcc
add_link_options("-rtlib=compiler-rt")
add_link_options("-nostdlib")

include_directories(${CMAKE_BINARY_DIR}/picolibc-install/include)

# Globally add the real builtins if RV32
add_link_options("-lclang_rt.builtins-riscv32")
