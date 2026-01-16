CMake Build System
==================

The Chimera-SDK build flow uses CMake to organize and compile code for both the host and cluster cores. Each component is separated into dedicated static libraries: ``runtime_host`` for host runtime functions, ``runtime_cluster_<type>`` for cluster runtime functions, and ``hal_host`` for the host HAL. We rely on Picolibc as the C standard library, which is compiled separately for host and cluster into ``picolibc_host`` and ``picolibc_cluster_<type>``. In addition, trampoline functions provide the minimal startup logic needed to bring up the cluster before the runtime and libraries are available. These functions run before the stack and global pointers are initialized, which is why they are implemented as naked functions without compiler-generated prologues or epilogues.

We separately compile code for the host and cluster cores into two object libraries, which are then currently linked together into a single executable.

Build Configuration
-------------------

ISA and ABI settings are handled by variables in the main ``CMakeLists.txt``:

- ``ABI`` – ABI for all code (e.g., ``ilp32`` or ``ilp32d``)
- ``ISA_HOST`` – ISA for host (e.g., ``rv32imc``)
- ``ISA_CLUSTER_SNITCH`` – ISA for cluster (e.g., ``rv32ima_xdma`` or ``rv32imafd_xdma``)


Building a Binary
-----------------

The build process involves creating separate object libraries for host and cluster code, linking them with the appropriate static libraries, and combining them into a unified executable.

Host Code
~~~~~~~~~

The host code is compiled into an object library and linked with the necessary static libraries. Source files for the host are typically organized in a ``src_host`` directory. The available libraries for host code are:

- ``hal_host`` - Hardware Abstraction Layer for host core
- ``runtime_host`` - Runtime functions for host core
- ``picolibc_host`` - Picolibc C library for host core
- ``trampoline_rv32im`` - Trampoline functions for using RV32IM

.. code-block:: cmake

    file(GLOB_RECURSE TEST_HOST_SRCS "src_host/*.c")
    add_library(${TEST_NAME}_host OBJECT ${TEST_HOST_SRCS})
    target_link_libraries(${TEST_NAME}_host PUBLIC
      hal_host
      runtime_host
      picolibc_host
      trampoline_rv32im
    )

Cluster Code
~~~~~~~~~~~~

The cluster code is compiled into an object library and linked with the necessary static libraries. Source files for the cluster are typically organized in a ``src_cluster`` directory. The available libraries for cluster code are:

- ``runtime_cluster_snitch`` - Runtime functions for Snitch cluster core
- ``picolibc_cluster_snitch`` - Picolibc C library for Snitch cluster core

.. code-block:: cmake

    file(GLOB_RECURSE TEST_CLUSTER_SRCS "src_cluster/*.c")
    add_library(${TEST_NAME}_cluster OBJECT ${TEST_CLUSTER_SRCS})
    target_link_libraries(${TEST_NAME}_cluster PUBLIC
      runtime_cluster_snitch
      picolibc_cluster_snitch
    )

.. important::
    Special attention is required for functions that execute before the cluster core is fully initialized, such as the trampoline function and interrupt handlers. At this stage, critical resources like the stack, global pointer, and thread pointer are not yet configured. Consequently, the compiler must not generate code that allocates stack frames. To address this, such functions are implemented as naked functions, which prevent the compiler from adding prologues or epilogues that rely on stack operations.

Unified Binary
~~~~~~~~~~~~~~

.. warning::
    The current approach compiles all code for both the host and cluster cores into a single library. This requires precise handling to ensure compatibility between the different instruction set architectures (ISAs) and application binary interfaces (ABIs). The ABI has to be identical to ensure correct function calls.
    This requires careful handling to avoid invalid instructions caused by mismatched ISAs between the host and cluster cores.

    To combine host and cluster into one executable:

    .. code-block:: cmake

        add_chimera_test(${TEST_NAME})
        target_link_libraries(${TEST_NAME} PUBLIC ${TEST_NAME}_host ${TEST_NAME}_cluster)

    .. tip::
        **It is recommended to always check the generated assembly code to ensure that the correct instructions are generated for the target core!**

