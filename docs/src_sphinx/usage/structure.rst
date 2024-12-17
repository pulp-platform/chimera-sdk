Folder Structure
================

The SDK is organized into the following main folders:

.. code-block:: none

    chimera-sdk
    ├─ cmake                                # CMake configuration and utility files
    ├─ devices                              # Device-specific code with potential external dependencies such as runtime libraries
    │ ├─ snitch_cluster
    │ │ ├─ trampoline_snitchCluster.c       # Trampoline code to offload functions to the cluster
    │ │ └─ snitch_runtime (external repo)   # External runtime library for the Snitch cluster
    │ └─ <more devices>
    ├─ drivers                              # Host drivers for peripherals and clusters
    │ ├─ cluster
    │ └─ <more drivers>
    ├─ hal                                  # Hardware Abstraction Layer (HAL)
    ├─ scripts                              # Utility scripts
    ├─ targets                              # Target definitions such as memory map, register definitions and build configurations
    │ ├─ chimera-open
    │ └─ <more targets>
    ├─ tests                                # Test applications for each target
    | ├─ chimera-open    
    | └─ <more targets>
    └─ CMakelists.txt                       # Top level CMakeLists.txt


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



