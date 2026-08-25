.. SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
.. SPDX-License-Identifier: CC-BY-ND-4.0

Test Vectors
============

The input and golden tensors used by the tests have to be **generated**. A fresh clone therefore has no test data, and the build will stop with a
message telling you to generate it.

Generate everything with::

    python scripts/generate_test_vectors.py

Run it once after cloning, and again whenever a generator changes.

This includes the per-shape ``ITA_dims_*.h`` headers, which declare which
shapes exist and drive the shape selection described below. Nothing under a
test's ``include/`` or ``src_cluster/`` is tracked once it is generated.

Test Vector Sources
-------------------

The vectors come from two places:

**Per-test generators.** Most tests ship a ``generate_vectors.py`` next to their
sources. These need only ``numpy``, seed it, and write the same files every
time. They also encode the kernel's reference model, so they double as
executable documentation of what the test checks.

**ITA's test-vector generator.** The ``ITA_MatMul_L1``, ``ITA_MatMul_L2`` and
``ITA_SHA_L2`` tests use data laid out in the tiled order the ITA accelerator
consumes. That layout is produced by ``testGenerator.py`` in
`pulp-platform/ITA <https://github.com/pulp-platform/ITA>`_, which is also what
feeds ITA's own RTL testbench. ``scripts/generate_test_vectors.py`` clones ITA into a temporary
directory, runs its generator for each required shape, and rewrites the
per-shape ``ITA_mem_<shape>.c`` files.

If you already have a checkout, point at it to skip the clone::

    python scripts/generate_test_vectors.py --ita-dir /path/to/ITA

To build only the tests that need no ITA checkout::

    python scripts/generate_test_vectors.py --skip-ita

Both sources are deterministic: the per-test generators seed numpy, and ITA's
``testGenerator.py`` seeds it too (``--seed``, default ``0``). Re-running
produces byte-identical output.

Regenerating from the build
---------------------------

Tests whose generator is registered with CMake can also be refreshed from the
build directory::

    cmake --build <build-dir> --target regenerate-test-vectors

This covers the per-test generators only; the ITA data needs the script above.

.. note::

   This target cannot bootstrap a fresh clone. Configuring stops as soon as it
   finds a missing vector, so the target does not exist yet at that point. Run
   ``scripts/generate_test_vectors.py`` first; the target is for refreshing
   vectors afterwards.

Selecting a shape
-----------------

Tests that ship several problem sizes pick one at configure time rather than by
commenting includes in and out::

    cmake -DITA_MATMUL_L1_SHAPE=64x512x64 ...

Available shapes are discovered from the ``ITA_dims_*.h`` (or
``testinputs_*.h``) headers a generator has written into the test, so adding a
new shape is a matter of teaching the generator about it. An unknown shape, or
one whose header and source disagree, triggers a configure-time error.

Adding a new generator
----------------------

Write a ``generate_vectors.py`` in the test directory that takes ``--output-*``
arguments defaulting to the paths the test includes, then register it in the
test's ``CMakeLists.txt``::

    chimera_test_vector_generator(
      GENERATOR ${CMAKE_CURRENT_SOURCE_DIR}/generate_vectors.py
      OUTPUTS
                ${CMAKE_CURRENT_SOURCE_DIR}/include/testinputs_16x16x16.h
                ${CMAKE_CURRENT_SOURCE_DIR}/include/testoutputs_16x16x16.h
    )

``OUTPUTS`` lists the files the generator writes; they are checked at configure
time and refreshed by the target above. Finally add the test directory to
``NUMPY_TESTS`` in ``scripts/generate_test_vectors.py`` and make sure the new
outputs are covered by the ``.gitignore`` patterns.
