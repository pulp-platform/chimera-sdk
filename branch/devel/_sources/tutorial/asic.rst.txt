.. SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
.. SPDX-License-Identifier: CC-BY-ND-4.0

ASIC Tutorial
=============

This tutorial is specific to the Chimera Convolve SoC, which is the first Chimera design tape-out.

This guide walks through building, flashing, and debugging a Chimera application on real
hardware.  The **One-Time Setup** section only needs to be done once per development machine.
After that you can jump straight to `Building and Running`_.


One-Time Setup
--------------

Prerequisites
^^^^^^^^^^^^^

Install the host tools needed to communicate with the board.

.. tab-set::

   .. tab-item:: macOS

      .. code-block:: bash

         brew install pipx automake
         pipx install pyserial
         pipx ensurepath

   .. tab-item:: Ubuntu

      .. code-block:: bash

         sudo apt update
         sudo apt install -y python3-pip git build-essential \
             pkg-config libtool autoconf automake texinfo
         pip install pyserial

Make sure Docker is installed and running.  Installation instructions are available on the
`Docker website <https://docs.docker.com/get-docker/>`_.  On Docker Desktop, enable host
networking under **Settings → Resources → Enable host networking**.

.. tip::

   Set ``CHIMERA_ROOT`` once and add it to your shell profile so every command below works
   without modification:

   .. code-block:: bash

      mkdir chimera && cd chimera
      export CHIMERA_ROOT=$(pwd)   # add to ~/.zshrc or ~/.bashrc

Install OpenOCD
^^^^^^^^^^^^^^^

OpenOCD communicates with the Chimera board over JTAG.

.. code-block:: bash

   cd $CHIMERA_ROOT
   git clone https://github.com/openocd-org/openocd.git
   cd openocd
   git checkout v0.12.0
   ./bootstrap
   ./configure --prefix=$CHIMERA_ROOT/install --disable-werror --enable-remote-bitbang
   make -j$(nproc)
   make install

Add ``$CHIMERA_ROOT/install/bin`` to your ``PATH`` if you used a local prefix.

Get the Repository
^^^^^^^^^^^^^^^^^^

.. code-block:: bash

   cd $CHIMERA_ROOT
   git clone https://github.com/pulp-platform/chimera-sdk.git
   cd chimera-sdk
   git submodule update --init --recursive

Get the Docker Container
^^^^^^^^^^^^^^^^^^^^^^^^

Pull the pre-built container and create a named instance with the repository mounted:

.. code-block:: bash

   docker pull ghcr.io/pulp-platform/chimera:devel

   docker run -it --name chimera --net=host \
       -v $CHIMERA_ROOT/chimera-sdk:/app/chimera \
       ghcr.io/pulp-platform/chimera:devel \
       zsh

The container ships with the LLVM toolchain, picolibc, ccache, Oh My ZSH (cypher theme),
``gdb-multiarch``, and all required Python dependencies.  No additional setup is needed.


Building and Running
--------------------

Start the Container
^^^^^^^^^^^^^^^^^^^

If the container is not running, start it and open a shell:

.. code-block:: bash

   docker start chimera
   docker exec -it chimera zsh

Build for ASIC
^^^^^^^^^^^^^^

Inside the container, configure for the ``chimera-convolve`` target with
``HARDWARE_BACKEND=ASIC`` and build:

.. code-block:: bash

   cd /app/chimera
   cmake -D TARGET_PLATFORM=chimera-convolve \
         -D TOOLCHAIN_DIR=/app/install/llvm-18.1.4-pulp \
         -D PICOLIBC_DIR=/app/install/picolibc \
         -D HARDWARE_BACKEND=ASIC \
         -D CHIMERA_UNIFIED_ELF=ON \
         -B build
   cmake --build build -j

Binaries land in ``build/bin/`` and additional build artifacts are in ``build/tests/``.

Connect via JTAG
^^^^^^^^^^^^^^^^

In a **separate terminal on the host**, run OpenOCD to establish a JTAG connection:

.. code-block:: bash

   cd $CHIMERA_ROOT/chimera-sdk
   openocd -f targets/chimera-convolve/chimera.openocd.hs2.tcl

A successful connection prints:

.. code-block:: text

   Info : Listening on port 3333 for gdb connections
   Ready for Remote Connections.

.. note::

   If you see ``Error: Fatal: Hart 0 failed to halt during examine()``, reset the chip and
   retry OpenOCD.

Open a Serial Terminal
^^^^^^^^^^^^^^^^^^^^^^

In another **host terminal**, open the serial port to see program output:

.. code-block:: bash

   pyserial-miniterm - 9600

Flash and Debug
^^^^^^^^^^^^^^^

With OpenOCD running, use ``gdb-multiarch`` **inside the container** to flash and debug:

.. code-block:: bash

   gdb-multiarch -iex "set auto-load safe-path /"

Connect to OpenOCD and load the binary:

.. code-block:: tcl

   target extended-remote localhost:3333
   monitor reset halt
   file build/bin/chimera_unified_<test>.elf
   load
   run

To re-run without restarting GDB:

.. code-block:: tcl

   load
   run
