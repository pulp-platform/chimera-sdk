.. SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
.. SPDX-License-Identifier: CC-BY-ND-4.0

Usage
=====

Building the SDK
----------------

The applications are built with RISC-V LLVM 18 or later.

.. important::
    For LLVM versions less than 15, linker relaxation is not supported and thus disabled.

.. tip::
    **Recommended for new users:** use the pre-built container (Docker or Singularity, see
    below).  The container ships with the correct LLVM toolchain, picolibc, and all Python
    dependencies pre-installed — no manual toolchain setup needed.

CMake Variables
^^^^^^^^^^^^^^^

All build methods use the same CMake variables:

- ``TARGET_PLATFORM`` – hardware target to build for (required; see `Targets`_)
- ``TOOLCHAIN_DIR`` – path to the LLVM installation root (required if not on ``PATH``)
- ``PICOLIBC_DIR`` – path to the prebuilt picolibc installation (required;
  see ``make picolibc-multilib``)
- ``HARDWARE_BACKEND`` – simulation back-end: ``RTL`` (default), ``GVSOC`` or ``ASIC``
- ``CHIMERA_UNIFIED_ELF`` – when ``ON``, merges device and host ELFs into a single
  mixed-ISA file suitable for simulation (requires ``lief``; default ``OFF``)

Docker (Recommended)
^^^^^^^^^^^^^^^^^^^^

The pre-built container at ``ghcr.io/pulp-platform/chimera:devel`` ships with a
PULP-flavoured RISC-V LLVM toolchain and all required dependencies.

.. code-block:: bash

    # Pull the container (one-time)
    docker pull ghcr.io/pulp-platform/chimera:devel

    # Start an interactive shell with the repository mounted at /app/chimera
    docker run -it --rm \
        -v $(pwd):/app/work \
        ghcr.io/pulp-platform/chimera:devel \
        zsh

    # Inside the container — configure and build
    cd /app/work
    cmake -D TARGET_PLATFORM=chimera-open \
          -D TOOLCHAIN_DIR=/app/install/llvm-18.1.4-pulp \
          -D PICOLIBC_DIR=/app/install/picolibc \
          -D HARDWARE_BACKEND=RTL \
          -D CHIMERA_UNIFIED_ELF=ON \
          -B build
    cmake --build build -j

Singularity (Recommended)
^^^^^^^^^^^^^^^^^^^^^^^^^

Singularity (or Apptainer) is available on IIS workstations and HPC clusters where Docker
is not permitted.

.. code-block:: bash

    # Pull the container (creates chimera_devel.sif in the current directory)
    singularity pull docker://ghcr.io/pulp-platform/chimera:devel

    # Start a clean shell with environment isolation (-e) inside the container
    singularity shell -e -s /bin/zsh chimera_devel.sif

    # Inside the container — configure and build
    cd /path/to/chimera-sdk
    cmake -D TARGET_PLATFORM=chimera-open \
          -D TOOLCHAIN_DIR=/app/install/llvm-18.1.4-pulp \
          -D PICOLIBC_DIR=/app/install/picolibc \
          -D HARDWARE_BACKEND=RTL \
          -D CHIMERA_UNIFIED_ELF=ON \
          -B build
    cmake --build build -j

.. note::
    The ``-e`` flag to ``singularity shell`` cleans the host environment so the container's
    tool paths take precedence.  Without it, host shell aliases or ``PATH`` entries can
    shadow the container's CMake or Python installations.

Generic Environment (Not Recommended)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you prefer a native installation, build the toolchain with ``make llvm`` and picolibc
with ``make picolibc-multilib``, then run:

.. code-block:: bash

    cmake -D TARGET_PLATFORM=chimera-open \
          -D TOOLCHAIN_DIR=<path-to-llvm> \
          -D PICOLIBC_DIR=<path-to-picolibc> \
          -D HARDWARE_BACKEND=RTL \
          -D CHIMERA_UNIFIED_ELF=ON \
          -B build
    cmake --build build -j

Targets
-------

The SDK supports multiple hardware targets.  Available targets are defined in
``targets/CMakeLists.txt`` under ``AVAILABLE_TARGETS``:

- ``chimera-convolve`` – target for the CONVOLVE project (RV64IMC host + RV32IMAFD cluster)
- ``chimera-host`` – host-only target (no cluster devices)
- ``chimera-open`` – default target with multiple Snitch clusters

Testing
-------

You can test functional correctness with the event-based simulator
`GVSoC <https://github.com/gvsoc/gvsoc>`_.  To install GVSoC:

.. code-block:: bash

    make gvsoc

By default, GVSoC is installed in ``install/gvsoc``.  Override with ``GVSOC_INSTALL_DIR``.

Run a test with the unified ELF:

.. code-block:: bash

    ./install/bin/gvsoc --target=chimera \
        --binary build/bin/chimera_unified_<test>.elf run