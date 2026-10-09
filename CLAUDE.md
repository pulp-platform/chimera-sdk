# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this
repository.

## Documentation

Full documentation lives in `docs/src_sphinx/` and is built with Sphinx.  Key pages:

- `docs/src_sphinx/usage/structure.rst` — repository layout
- `docs/src_sphinx/usage/build.rst` — CMake API (`add_device_binary` / `add_host_binary`)
- `docs/src_sphinx/usage/multi_binary.rst` — design of the multi-binary compilation flow
- `docs/src_sphinx/usage/usage.rst` — build instructions for all environments
- `docs/multi_binary_compilation.md` — standalone design reference

## Building (Docker)

All dependencies are pre-installed in the container.  This is the standard way to build.

```bash
docker run -it --rm \
    -v $(pwd):/app/chimera \
    ghcr.io/pulp-platform/deeploy:devel \
    zsh

# Inside the container:
cd /app/chimera
cmake -D TARGET_PLATFORM=chimera-open \
      -D TOOLCHAIN_DIR=/app/install/llvm-18.1.4-pulp \
      -D PICOLIBC_DIR=/app/install/picolibc \
      -D HARDWARE_BACKEND=RTL \
      -D CHIMERA_UNIFIED_ELF=ON \
      -B build
cmake --build build -j
```

A chimera-specific Docker image (LLVM + compiler-rt + picolibc, no emulators) can be built
with `make container` — see `container/README.md` for details (requires SSH access to
`iis-git.ee.ethz.ch`).

For other environments (Singularity, native, IIS workstations) see `docs/src_sphinx/usage/usage.rst`.

## Adding a New Test

Copy the pattern from `tests/snitchCluster/simpleOffload/CMakeLists.txt`.  For host-only
tests omit `DEVICE_DEPS`, `DEVICE_SYMBOLS`, and `LAST_DEVICE`.

## Non-Obvious CMake Mechanics

**Per-binary runtime compilation.** Each target's `CMakeLists.txt` registers runtime sources
via `set_property(GLOBAL PROPERTY CHIMERA_HOST_RUNTIME_SOURCES …)` and
`CHIMERA_DEVICE_RUNTIME_SOURCES`.  `add_device_binary` / `add_host_binary` compile these
sources into a private object library per binary using that binary's ISA/ABI.  Changing a
runtime source file in `targets/<target>/` re-triggers recompilation for every binary.

**Placement chain.** The linker script templates contain `@CHIMERA_PREV_INCLUDE@` and
`@CHIMERA_RESERVED_SECTION@` placeholders that CMake substitutes at configure time based on
`PREV_DEVICE` / `LAST_DEVICE` arguments.  If the memory layout looks wrong, check the
generated `*_link.ld` files in `build/`.

## LSP / IDE Notes

Expect false-positive diagnostics — they do not affect the build:

- `rv32ima_xdma` and `rv32imafd_xdma` are PULP-internal ISA extensions unknown to system Clang.
- Picolibc and `shared.h` include paths are injected by CMake at build time and are not visible to the LSP.

## Code Conventions

- All files require an SPDX header (`// SPDX-FileCopyrightText:` / `# SPDX-…`).
- C: `clang-format` (`.clang-format` at root).
- Python: `yapf` + `isort`.
- Run `pre-commit run --all-files` (or `make format`) before pushing.
