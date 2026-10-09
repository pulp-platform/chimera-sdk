.. SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
.. SPDX-License-Identifier: CC-BY-ND-4.0

Folder Structure
================

The SDK is organised into the following main folders:

.. code-block:: none

    chimera-sdk/
    ├─ cmake/                           # CMake API and helper scripts
    │  ├─ Chimera.cmake                 # add_device_binary() + add_host_binary()
    │  ├─ scripts/
    │  │  ├─ ChimeraBuildHelpers.cmake  # symbol extraction + placement generation
    │  │  └─ PrintBuildFooter.cmake     # build summary output
    │  ├─ toolchain_llvm.cmake          # LLVM toolchain configuration
    │  └─ Utils.cmake                   # utility macros
    ├─ devices/                         # Device-specific runtime code
    │  └─ snitch_cluster/
    │     ├─ runtime/                   # Snitch cluster runtime library
    │     └─ third_party/               # snitch-sdk and other external deps
    ├─ host/                            # Host core code
    │  ├─ drivers/                      # Peripheral drivers
    │  │  ├─ cluster/                   # Cluster offload API (offload_snitchCluster)
    │  │  ├─ uart_abb/
    │  │  └─ <more drivers>
    │  ├─ hal/                          # Hardware Abstraction Layer
    │  ├─ runtime/                      # Host runtime functions
    │  └─ third_party/                  # OpenTitan drivers
    ├─ shared/                          # Cross-cutting utilities
    │  └─ inc/macros.h                  # Common preprocessor macros
    ├─ scripts/                         # Python utility scripts
    │  ├─ check_section_overlaps.py     # Memory map verification
    │  └─ merge_elf.py                  # Mixed-ISA ELF merging (lief)
    ├─ targets/                         # Per-target configuration
    │  ├─ chimera-convolve/             # CONVOLVE project target
    │  │  ├─ shared/inc/               # shared.h (chimera_shared_data_t)
    │  │  ├─ host/                      # crt0.S, link.ld.in (host linker script)
    │  │  └─ devices/snitch_cluster/   # crt0.S, trampoline.c/h, link.ld.in
    │  ├─ chimera-open/                 # Default multi-cluster target
    │  │  ├─ shared/inc/
    │  │  ├─ host/
    │  │  └─ devices/snitch_cluster/
    │  └─ chimera-host/                 # Host-only target (no cluster devices)
    │     └─ host/
    ├─ tests/                           # Test applications
    │  ├─ host/                         # Host-only tests (no cluster)
    │  │  ├─ returnZero/
    │  │  ├─ alloc/
    │  │  ├─ printf/
    │  │  └─ uartSimple/
    │  ├─ snitchCluster/                # Host + Snitch cluster tests
    │  │  ├─ simpleOffload/             # Minimal offload example
    │  │  ├─ matmul/
    │  │  ├─ hyperbus/
    │  │  └─ snrt/
    │  └─ chimera-convolve/             # CONVOLVE-specific tests
    └─ CMakeLists.txt                   # Top-level CMake entry point

.. note::
    Peripheral drivers are selected by the ``DRIVER_MAPPINGS`` variable in the build system.
    This allows drivers to be included or excluded per target without modifying test code.

Target Layout
-------------

Each entry in ``targets/`` provides the memory map, linker scripts, startup code, and
shared data definitions for one hardware configuration:

.. code-block:: none

    targets/<target>/
    ├─ CMakeLists.txt                   # sets CHIMERA_HOST_CRT0, CHIMERA_DEVICE_CRT0,
    │                                   # CHIMERA_HOST_LINKER_SCRIPT,
    │                                   # CHIMERA_DEVICE_LINKER_SCRIPT,
    │                                   # CHIMERA_COMMON_LDH_DIR
    ├─ config.cmake                     # ISA/ABI/toolchain variables for this target
    ├─ shared/inc/shared.h              # chimera_shared_data_t definition
    ├─ host/
    │  ├─ crt0.S                        # Host startup code
    │  └─ link.ld.in                    # Host linker script template
    └─ devices/snitch_cluster/
       ├─ crt0.S                        # Cluster startup code
       ├─ link.ld.in                    # Device linker script template
       ├─ trampoline.c                  # Cluster entry point (naked function)
       └─ trampoline.h                  # Trampoline macros

Tests
-----

Each test has its own directory with a ``CMakeLists.txt`` that calls
``add_device_binary()`` and ``add_host_binary()``.  Source files are split by domain:

.. code-block:: none

    tests/snitchCluster/<testName>/
    ├─ CMakeLists.txt                   # calls add_device_binary + add_host_binary
    ├─ src_cluster/                     # device source files (rv32)
    │  └─ main.c
    └─ src_host/                        # host source files (rv64)
       └─ main.c

Host-only tests follow the same pattern but call ``add_host_binary()`` without any
``DEVICE_DEPS``, ``DEVICE_SYMBOLS``, or ``LAST_DEVICE`` arguments:

.. code-block:: none

    tests/host/<testName>/
    ├─ CMakeLists.txt                   # calls add_host_binary only
    └─ src/
       └─ main.c
