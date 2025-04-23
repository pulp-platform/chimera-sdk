HAL Communication Interface
***************************

A communication interface is represented by `chi_interface_t` and exposes
`open`, `close`, `read`, and `write` methods for UART, SPI, etc.

Communication Interface API
===========================

.. doxygengroup:: hal_interface
    :content-only:
    :members:
    :protected-members:
    :project: chimera_hal

UART HAL
~~~~~~~~

.. doxygengroup:: hal_uart
    :content-only:
    :members:
    :protected-members:
    :project: chimera_hal
