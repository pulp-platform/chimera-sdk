CMake Build System
==================

All runtime functions executed by the host core are compiled into a dedicated ``runtime_host`` static library and the cluster code into ``runtime_cluster_<type>`` (e.g. ``runtime_cluster_snitch``). Additionally, the HAL layer is compiled into the ``hal_host`` static libary.
The final binary is seperated into two object libaries, one for the host and one for the cluster core. The host object library is linked with the ``runtime_host`` and ``hal_host`` libraries, while the cluster object library is linked with the ``runtime_cluster_<type>`` library. The final binary is then linked from the two object libraries.


Mixed ISA Compilation
---------------------
The current approach compiles all code for both the host and cluster cores into a single library. This requires precise handling to ensure compatibility between the different instruction set architectures (ISAs) and application binary interfaces (ABIs).
This requires careful handling to avoid invalid instructions caused by mismatched ISAs between the host and cluster cores. Hence, we define four CMake variables, ``ABI``, ``ISA_HOST``, and ``ISA_CLUSTER_SNITCH``, to specify the appropriate ISA for each core type. The ABI has to be identical to ensure correct function calls.
Furthermore, the tests are split into ``src_host`` and ``src_cluster`` directories to clearly separate code executed on the host and cluster cores. 


.. important::
    Special attention is required for functions that execute before the cluster core is fully initialized, such as the trampoline function and interrupt handlers. At this stage, critical resources like the stack, global pointer, and thread pointer are not yet configured. Consequently, the compiler must not generate code that allocates stack frames. To address this, such functions are implemented as naked functions, which prevent the compiler from adding prologues or epilogues that rely on stack operations.

.. tip::
    **It is recommended to always check the generated assembly code to ensure that the correct instructions are generated for the target core!**
