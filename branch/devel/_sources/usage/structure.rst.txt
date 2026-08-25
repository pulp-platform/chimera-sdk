.. SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
.. SPDX-License-Identifier: CC-BY-ND-4.0

Folder Structure
================

The SDK is organized into the following main folders:

.. code-block:: none

    chimera-sdk
    ├─ cmake                                # CMake configuration and utility files
    ├─ devices                              # Device-specific code with potential external dependencies such as runtime libraries
    │ ├─ snitch_cluster
    │ │ ├─ runtime                          # Runtime library for the Snitch cluster
    │ │ ├─ third_party                      # External dependencies such as snitch-sdk
    │ │ ├─ trampoline_snitchCluster.c       # Trampoline code to offload functions to the cluster
    │ │ └─ snitch_runtime (external repo)   # External runtime library for the Snitch cluster
    │ └─ <more devices>
    ├─ host                                 # Host code with drivers, HAL, runtime and external dependencies
    │ ├─ drivers                            # Drivers for peripherals and devices such as clusters and accelerators
    │ │ ├─ cluster
    │ │ ├─ uart_abb
    │ │ └─ <more drivers>
    │ ├─ hal                                # Hardware Abstraction Layer (HAL)
    │ ├─ runtime                            # Runtime library for the host
    │ ├─ third_party                        # External dependencies such as opentitan drivers
    ├─ scripts                              # Utility scripts
    ├─ targets                              # Target definitions such as memory map, register definitions and build configurations
    │ ├─ chimera-open
    │ └─ <more targets>
    ├─ tests                                # Test applications for each target
    | ├─ chimera-open
    | └─ <more targets>
    └─ CMakelists.txt                       # Top level CMakeLists.txt

.. note::
    The drivers are selected by the ``DRIVER_MAPPINGS`` variable
    in the build system. This allows to easily include or exclude drivers as needed.

Tests
-----
Each target has a corresponding folder in the ```tests``` directory. The tests are organized by category, such as ``host`` or ``snitchCluster``. Each category contains multiple tests. The tests are separated into host and cluster code.

.. code-block:: none

    tests
    ├─ chimera-open                   # Test applications for the chimera-open target
    | ├─ <category>                   # Test categories (what part of the system is tested, e.g host, snitchCluster)
    | │ ├─ test_1
    | │ │ ├─ host                     # Host code
    | │ │ └─ snitchCluster            # Snitch sluster code
    | │ └─ <more tests>
    | └─ <more categories>
    └─ <more targets>



