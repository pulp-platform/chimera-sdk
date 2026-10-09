.. SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
.. SPDX-License-Identifier: CC-BY-ND-4.0

HAL Interrupt Controller
*************************

An interrupt controller is represented by the `chi_interrupt_t` structure and
provides methods to register handlers, enable/disable lines, acknowledge events,
and dispatch.

.. doxygengroup:: hal_interrupt
    :content-only:
    :members:
    :protected-members:
    :project: chimera_hal

