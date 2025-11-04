.. Copyright 2025 ETH Zurich and University of Bologna.
.. SPDX-License-Identifier: CC-BY-ND-4.0

CLINT
-----

.. doxygengroup:: drivers_clint
   :project: chimera_ll
   :members:

64-bit CLINT Driver
+++++++++++++++++++++

The 64-bit CLINT driver implements timer and interrupt functionalities using a 64-bit data type (`uint64_t`) for the mtime register. This configuration is activated when the `PLATFORM_IS_64BIT` flag is defined.

.. doxygengroup:: drivers_clint_64
   :project: chimera_ll
   :members:

32-bit CLINT Driver
+++++++++++++++++++++

The 32-bit CLINT driver adapts the timer and sleep functionalities for 32-bit platforms by representing the mtime register as a structure with two 32-bit members (`low` and `high`). This approach maintains compatibility with 32-bit systems while delivering equivalent performance and functionality as the 64-bit version.

.. doxygengroup:: drivers_clint_32
   :project: chimera_ll
   :members: