# Copyright 2024 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Moritz Scherer <scheremo@iis.ee.ethz.ch>

SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
DOXYOGENPOTS  ?=
DOXYGENBUILD  ?= doxygen

SOURCEDIR_SPHINX     = src_sphinx
BUILDDIR_SPHINX      = _build_sphinx

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR_SPHINX)" "$(BUILDDIR_SPHINX)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile doxygen

doxygen: 
	@echo "Building Doxygen documentation"
	@$(DOXYGENBUILD) $(DOXYOGENPOTS) Doxyfile_HAL.in 
	@$(DOXYGENBUILD) $(DOXYOGENPOTS) Doxyfile_LL_chimera-open.in

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile | doxygen
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR_SPHINX)" "$(BUILDDIR_SPHINX)" $(SPHINXOPTS) $(O)

clean:
	rm -rf _build_*
	@$(SPHINXBUILD) -M clean "$(SOURCEDIR_SPHINX)" "$(BUILDDIR_SPHINX)" $(SPHINXOPTS) $(O)