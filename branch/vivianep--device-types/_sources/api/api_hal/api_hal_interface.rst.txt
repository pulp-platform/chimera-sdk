.. SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
.. SPDX-License-Identifier: CC-BY-ND-4.0

HAL Communication Interface
***************************

A communication interface is represented by `chi_interface_t` and exposes
`open`, `close`, `read`, and `write` methods for UART, SPI, etc.

.. doxygengroup:: hal_interface
    :content-only:
    :members:
    :protected-members:
    :project: chimera_hal
