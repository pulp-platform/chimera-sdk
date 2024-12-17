Usage
=====

Building the SDK
----------------

To build the SDK and all tests contained in the SDK, run:

.. code-block:: bash

    mkdir build && cd build
    cmake -DTARGET_PLATFORM=[YOURTARGETPLATFORM] ../
    cmake --build .

where you should replace ``[YOURTARGETPLATFORM]`` by one of the platforms defined in ``targets/CMakeLists.txt`` under ``AVAILABLE_TARGETS``.
The resulting binaries will be stored in ``build/bin``, and can be used within the ``chimera`` repo as tests.

The applications are built using the RISC-V LLVM toolchain. On IIS systems, users can use the pre-installed LLVM version 12.0.1.
Outside of IIS systems, you need to install LLVM version 12.0.1 or later to ensure compatibility. Thus you need to specify the ``TOOLCHAIN_DIR`` parameter when running cmake. 

.. code-block:: bash

    mkdir build && cd build
    cmake -DTARGET_PLATFORM=[YOURTARGETPLATFORM] -DTOOLCHAIN_DIR=/usr/pack/riscv-1.0-kgf/pulp-llvm-0.12.0/bin ../
    cmake --build .

The correct version of the toolchain can be verified by running

.. code-block:: bash

    llvm-config --version


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
            "TOOLCHAIN_DIR": "/usr/pack/riscv-1.0-kgf/pulp-llvm-0.12.0/bin",
            "TARGET_PLATFORM": "chimera-convolve",
        },
        "cmake.environment": {
            "PATH": "/usr/pack/riscv-1.0-kgf/default/bin:${env:PATH}",
            "LD_LIBRARY_PATH": "/usr/pack/riscv-1.0-kgf/lib64:/usr/pack/riscv-1.0-kgf/lib64",
        }
    }

If you are not on an IIS system, you need to adjust the paths according to your local installation.
