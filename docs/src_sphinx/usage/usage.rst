Usage
=====

Building the SDK
----------------

The applications are built with RISC-V LLVM 12.0.1 or later to ensure compatibility.

.. important::
    For LLVM versions less than 15, linker relaxation is not supported and thus disabled.


Generic Environment
^^^^^^^^^^^^^^^^^^^

To build the SDK and all tests contained in the SDK, run:

.. code-block:: bash

    cmake -DTARGET_PLATFORM=<target> -B build
    cmake --build build -j

where you should replace ``[YOURTARGETPLATFORM]`` by one of the platforms defined in ``targets/CMakeLists.txt`` under ``AVAILABLE_TARGETS``.
The resulting binaries will be stored in ``build/bin``, and can be used within the ``chimera`` repo as tests.

If you did not globally install the toolchain, you need to specify the ``TOOLCHAIN_DIR`` parameter when running cmake.

.. code-block:: bash

    cmake -DTARGET_PLATFORM=<target> -DTOOLCHAIN_DIR=<path-to-toolchain> ../
    cmake --build build -j

IIS Workstations
^^^^^^^^^^^^^^^^

On IIS systems, users can use the pre-installed LLVM compiler by activating the riscv environment with the ``riscv`` command.
This command sets the necessary environment variables for the toolchain.
To build the SDK, run:

.. code-block:: bash

    riscv zsh # Setup the default riscv environment (modifies PATH and LD_LIBRARY_PATH)
    cmake -DTARGET_PLATFORM=<target> -DTOOLCHAIN_DIR=/usr/pack/riscv-1.0-kgf/pulp-llvm-0.12.0 -B build
    cmake --build build -j


Targets
-------
The SDK supports multiple targets, each with a different configuration. The available targets are defined in ``targets/CMakeLists.txt`` under ``AVAILABLE_TARGETS``. The following targets are currently available:

- ``chimera-convolve``: Target for the CONVOLVE project.
- ``chimera-host``: Target with a single host core without clusters.
- ``chimera-open``: Default target with multiple clusters.


Visual Studio Code Integration
------------------------------
To enable automatic configuration of the C/C++ extension and support for the integrated cMake build flow on the IIS workstations, add the following content to ``.vscode/settings.json``:

.. code-block:: json

    {
        "cmake.configureSettings": {
            "TOOLCHAIN_DIR": "/usr/pack/riscv-1.0-kgf/pulp-llvm-0.12.0",
            "TARGET_PLATFORM": "chimera-convolve",
        },
    }

If you are not on an IIS system, you need to adjust the paths according to your local installation.
