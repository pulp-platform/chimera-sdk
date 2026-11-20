API References
**************

The Chimera project is broken into several different abstraction layers, each of which has its own API reference. The following sections provide a high-level overview of the different APIs available in the Chimera project.

The Low Level (LL) API provides a direct interface to the hardware of the SoC and is designed to be close to the hardware, providing a thin abstraction layer to make the hardware easier to use. It contains all runtime drivers and peripheral interfaces.
The Hardware Abstraction Layer (HAL) API provides a higher-level interface to the hardware of the SoC and is designed to be more user-friendly than the LL API.
Finally, the CMake API provides a set of CMake functions and macros that can be used to build and configure the Chimera project.

.. toctree::
   :maxdepth: 1

   api_hal/api_hal
   api_ll/api_ll
   api_cmake/api_cmake


