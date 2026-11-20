.. SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
.. SPDX-License-Identifier: CC-BY-ND-4.0

Multi-Binary Compilation Flow
==============================

Chimera-SDK compiles **one ELF per execution domain**.  The CVA6 host core and
each Snitch cluster are built with their own ISA, ABI, and linker script, then
placed at non-overlapping regions of a shared address space (``memisl``).

Motivation
----------

The original chimera-sdk compiled host and all cluster code into a **single ELF**.
This creates two real problems:

1. **ISA/ABI mismatch risk.** The CVA6 host runs RV64IMC; a Snitch cluster runs
   RV32IMAFD.  Linking into one binary requires a common-denominator ISA, which
   restricts optimisations and can silently produce incorrect code for one domain.
2. **No clean separation.** Host code must not call cluster functions directly
   (wrong ISA), yet a shared link namespace makes such accidents easy and hard to
   detect at compile time.

The new approach produces one ELF per domain and uses a thin symbol-stub layer so
the host can reference cluster code by name without any ISA confusion.

Architecture Overview
---------------------

.. code-block:: none

          ┌───────────────────────────────────────────────┐
          │                 memisl (shared SRAM)          │
          │  0x48000000                                    │
          │  ┌──────────────┐                             │
          │  │  .common     │  chimera_shared_data_t       │
          │  │  (64-byte    │  host owns / initialises;   │
          │  │   aligned)   │  devices access via AXI     │
          │  ├──────────────┤                             │
          │  │ Device 0 ELF │  RV32IMAFD  (Snitch)        │
          │  ├──────────────┤ ← __device0_end (4K-aligned)│
          │  │  Host ELF    │  RV64IMC    (CVA6)          │
          │  └──────────────┘                             │
          │  ORIGIN + LENGTH  ← host stack (grows down)   │
          └───────────────────────────────────────────────┘

Both domains access the ``.common`` section at the same physical address
(``0x48000000``).  The host binary loads and initialises it; device binaries
mark it ``NOLOAD`` and read it over the AXI interconnect.

The Three-Stage Pipeline
------------------------

.. code-block:: none

   ┌─────────────────────────────────────────────────────────────────┐
   │  Stage 1: Compile device binary                                 │
   │                                                                 │
   │  device sources + crt0.S + runtime ──[clang rv32imafd]──►      │
   │                  device.elf   (position-independent, rv32)      │
   └──────────────────────────┬──────────────────────────────────────┘
                              │
                              ▼
   ┌─────────────────────────────────────────────────────────────────┐
   │  Stage 2: Extract public symbols                                │
   │                                                                 │
   │  llvm-nm device.elf ──► ChimeraBuildHelpers.cmake              │
   │                          ├─► device_symbols.s                  │
   │                          │     .global device_main             │
   │                          │     .set    device_main, 0x48001234 │
   │                          ├─► device_symbols.h                  │
   │                          │     extern char device_main[];      │
   │                          └─► device_placement.ldh              │
   │                                __device_end = 0x48010000;      │
   └──────────────────────────┬──────────────────────────────────────┘
                              │
                              ▼
   ┌─────────────────────────────────────────────────────────────────┐
   │  Stage 3: Compile host binary                                   │
   │                                                                 │
   │  host sources + crt0.S + runtime + device_symbols.s            │
   │  ──[clang rv64imc]──► host.elf                                  │
   │                                                                 │
   │  Host linker script INCLUDEs device_placement.ldh so host      │
   │  .text is placed immediately after device code.                 │
   └─────────────────────────────────────────────────────────────────┘

Stages 1 and 2 are driven by :cmake:command:`add_device_binary`; Stage 3 is
driven by :cmake:command:`add_host_binary`.  CMake dependency tracking ensures
the stages run in order and that the host ELF relinks whenever the device
changes.

Shared Communication: The ``.common`` Section
----------------------------------------------

All binaries map the same physical address (``0x48000000``) for shared state.
The layout is defined by a single C struct compiled into every binary:

.. code-block:: c

    /* targets/<target>/shared/inc/shared.h */
    typedef struct __attribute__((packed)) {
        volatile uint64_t host_to_device;
        volatile uint64_t device_to_host;
        volatile uint32_t host_to_device_flag[NUM_CLUSTERS];
        volatile uint32_t device_to_host_flag[NUM_CLUSTERS];
        volatile uint32_t trampoline_function[NUM_CLUSTER_CORES];
        volatile uint32_t trampoline_args[NUM_CLUSTER_CORES];
        volatile uint32_t trampoline_stack[NUM_CLUSTER_CORES];
    } chimera_shared_data_t;

    extern chimera_shared_data_t shared_data;

The instance is defined in ``shared.c``, placed in ``.common`` at 64-byte
alignment.  The host linker script loads this section normally:

.. code-block:: none

    .common : ALIGN(64) { KEEP(*(.common)) } > memisl AT > memisl

Device linker scripts mark it ``NOLOAD`` — the memory is already initialised by
the host and must not be overwritten:

.. code-block:: none

    .common (NOLOAD) : ALIGN(64) { KEEP(*(.common)) } > memisl

Because all binaries compile the same ``shared.c`` with the same struct
definition, ``offsetof()`` values are identical.  The trampoline code uses
``offsetof`` to access struct fields before the global pointer is set up.

Memory Placement: The Placement Chain
--------------------------------------

After each device ELF is linked, ``ChimeraBuildHelpers.cmake`` writes a small
linker header recording its 4 KiB-aligned end address:

.. code-block:: none

    /* device_placement.ldh — auto-generated */
    __device_end = 0x48010000;

The next binary INCLUDEs this file and skips over the occupied region with a
NOLOAD ``.reserved`` section.  This forms a **placement chain**:

.. code-block:: none

    Device 0 → device0_placement.ldh → Device 1 → device1_placement.ldh → Host

CMake builds this chain automatically from the ``PREV_DEVICE`` and
``LAST_DEVICE`` arguments to :cmake:command:`add_device_binary` and
:cmake:command:`add_host_binary`.

Symbol Extraction Pipeline
--------------------------

After Stage 1, ``ChimeraBuildHelpers.cmake`` runs ``llvm-nm`` on the device ELF
and applies the following filter rules:

- Only TEXT (``T``/``t``), DATA (``D``/``d``), BSS (``B``/``b``), and RODATA
  (``R``/``r``) symbols are exported.
- Symbols whose name starts with ``__`` (double underscore) are skipped — these
  are linker-internal bookkeeping symbols (``__bss_start``, ``__device_end``,
  etc.) that must not appear in the host's link namespace.
- Undefined and weak symbols are not exported.

The ``DEVICE_NAME`` prefix (e.g. ``device``) is prepended to every exported
symbol name.  This prevents link-time collisions when multiple devices define
the same symbol (e.g. ``main``).

The generated assembly stub is assembled with the **host** toolchain and
wrapped in a CMake INTERFACE library (``${TARGET_NAME}_symbols``) for use with
:cmake:command:`add_host_binary`.

CMake Build Configuration
--------------------------

ISA and ABI settings are established by target-specific CMake variables (set in
``targets/<target>/CMakeLists.txt``):

- ``ISA_HOST`` – host march string (e.g. ``rv64imc``)
- ``ABI_HOST`` – host mabi string (e.g. ``lp64``)
- ``ISA_CLUSTER_SNITCH`` – device march string (e.g. ``rv32imafd``)
- ``ABI_CLUSTER_SNITCH`` – device mabi string (e.g. ``ilp32d``)

``add_device_binary()``
------------------------

Compiles a device binary and creates the ``${TARGET_NAME}_symbols`` INTERFACE library
consumed by ``add_host_binary()``.

.. code-block:: cmake

    add_device_binary(${TEST_NAME}_device
        DEVICE_NAME  device
        ISA          ${ISA_CLUSTER_SNITCH}
        ABI          ${ABI_CLUSTER_SNITCH}
        COMPILER     ${CROSS_COMPILE_DEVICE_SNITCH_CLUSTER}
        COMPIERT_RT  ${COMPILERRT_CLUSTER_SNITCH}
        PICOLIBC     ${PICOLIB_CLUSTER_SNITCH}
        TARGET_DIR   ${CHIMERA_TARGET_DIR}
        DEVICE_DIR   ${CHIMERA_TARGET_DIR}/devices/snitch_cluster
        SOURCES      ${TEST_SNITCH_SRCS}
    )

Key arguments:

- ``DEVICE_NAME`` – short name prepended to every exported symbol (e.g. ``device_main``).
  Must be unique across all devices in a build to avoid link-time collisions in the host.
- ``TARGET_DIR`` / ``DEVICE_DIR`` – directories containing ``common.ldh`` and the
  ``link.ld.in`` linker script template respectively.
- ``PREV_DEVICE`` – optional; name of the preceding device in memisl order.  When set, this
  device is placed immediately after it via the placement chain.

``add_host_binary()``
----------------------

Compiles the host binary, links the device symbol stubs, and runs post-build checks.

.. code-block:: cmake

    add_host_binary(${TEST_NAME}
        ISA          ${ISA_HOST}
        ABI          ${ABI_HOST}
        COMPILER     ${CROSS_COMPILE_HOST}
        COMPIERT_RT  ${COMPILERRT_HOST}
        PICOLIBC     ${PICOLIB_HOST}
        TARGET_DIR   ${CHIMERA_TARGET_DIR}
        DEVICE_DIR   ${CHIMERA_TARGET_DIR}/host
        SOURCES      ${TEST_HOST_SRCS}
        DEVICE_DEPS    ${TEST_NAME}_device
        DEVICE_SYMBOLS ${TEST_NAME}_device_symbols
        LAST_DEVICE    ${TEST_NAME}_device
    )

Key arguments:

- ``DEVICE_DEPS`` – device ELF targets that must finish building before symbol extraction
  runs (build ordering only; no code is pulled from these).
- ``DEVICE_SYMBOLS`` – ``${dev}_symbols`` INTERFACE libraries produced by
  ``add_device_binary()``.  Their compiled object files (the ``.set`` stubs) are linked into
  the host ELF so device symbols resolve to their absolute addresses.
- ``LAST_DEVICE`` – the device at the tail of the placement chain.  Its
  ``_placement.ldh`` is included by the host linker script via
  ``@CHIMERA_PLACEMENT_INCLUDE@``, so host code is placed immediately after all device
  code.  Leave empty or omit for host-only builds (the placeholder becomes an empty string).

Minimal Test CMakeLists.txt
----------------------------

A new test in ``tests/snitchCluster/<testName>/`` typically looks like:

.. code-block:: cmake

    set(TEST_NAME test_snitchCluster_myTest)

    file(GLOB_RECURSE TEST_SNITCH_SRCS "src_cluster/*.c")
    file(GLOB_RECURSE TEST_HOST_SRCS   "src_host/*.c")

    add_device_binary(${TEST_NAME}_device
        DEVICE_NAME  device
        ISA          ${ISA_CLUSTER_SNITCH}
        ABI          ${ABI_CLUSTER_SNITCH}
        COMPILER     ${CROSS_COMPILE_DEVICE_SNITCH_CLUSTER}
        COMPIERT_RT  ${COMPILERRT_CLUSTER_SNITCH}
        PICOLIBC     ${PICOLIB_CLUSTER_SNITCH}
        TARGET_DIR   ${CHIMERA_TARGET_DIR}
        DEVICE_DIR   ${CHIMERA_TARGET_DIR}/devices/snitch_cluster
        SOURCES      ${TEST_SNITCH_SRCS}
    )

    add_host_binary(${TEST_NAME}
        ISA          ${ISA_HOST}
        ABI          ${ABI_HOST}
        COMPILER     ${CROSS_COMPILE_HOST}
        COMPIERT_RT  ${COMPILERRT_HOST}
        PICOLIBC     ${PICOLIB_HOST}
        TARGET_DIR   ${CHIMERA_TARGET_DIR}
        DEVICE_DIR   ${CHIMERA_TARGET_DIR}/host
        SOURCES      ${TEST_HOST_SRCS}
        DEVICE_DEPS    ${TEST_NAME}_device
        DEVICE_SYMBOLS ${TEST_NAME}_device_symbols
        LAST_DEVICE    ${TEST_NAME}_device
    )

See ``tests/snitchCluster/simpleOffload/CMakeLists.txt`` for a complete working example.

Trampoline and Naked Functions
-------------------------------

.. important::
    Functions that execute before the cluster core is fully initialised — such as the
    trampoline entry point and interrupt handlers — run before the stack pointer and global
    pointer are set up.  These must be implemented as **naked functions** so the compiler
    does not generate a stack frame prologue or epilogue.

The trampoline is a naked C function in the **device** binary that the host uses to
launch workloads on a cluster core.  It reads ``sp``, ``fn``, and ``args`` from
``shared_data`` fields using ``offsetof()``-computed offsets, which avoids any reliance on
GP-relative addressing:

.. code-block:: c

    __attribute__((naked)) void _trampoline(void) {
        asm volatile(
            "lw  sp, %0(a0)\n"
            "lw  t0, %1(a0)\n"
            "jalr t0\n"
            :
            : "i"(offsetof(chimera_shared_data_t, trampoline_stack)),
              "i"(offsetof(chimera_shared_data_t, trampoline_function))
        );
    }

The trampoline lives in the **device** binary only and is never linked into the host.

Key Design Decisions
--------------------

.. list-table::
   :widths: 30 70
   :header-rows: 1

   * - Decision
     - Rationale
   * - Separate ELFs, not libraries
     - A static library compiled for ``rv32imafd`` cannot be safely linked into
       an ``rv64imc`` host — the linker accepts it, but the instructions are
       wrong.  Separate ELFs make the ISA boundary explicit and enforced.
   * - ``.common`` is NOLOAD on device
     - The host initialises ``.common`` at startup.  If devices also loaded it,
       the initialisation would be overwritten.  NOLOAD means the device never
       zeros that region.
   * - Symbol stubs, not ``EXTERN`` in linker scripts
     - Absolute ``.set`` symbols in a compiled object resolve at final link
       without relocation.  ``EXTERN`` only pulls in a symbol if it is otherwise
       unreferenced, which is order-dependent.  The stub approach is reliable
       regardless of link order.
   * - ``DEVICE_NAME`` prefix
     - Two devices both export ``main``.  Without a prefix the host link
       resolves both to the same symbol.  The prefix makes each symbol unique
       in the host namespace.
   * - 4 KiB-aligned ``__device_end``
     - Page-aligned boundaries simplify MPU/PMP configuration and prevent
       accidental cache-line sharing between adjacent binaries.
