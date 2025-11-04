# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

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
	@$(DOXYGENBUILD) $(DOXYOGENPOTS) Doxyfile_HOST_HAL.in
	@$(DOXYGENBUILD) $(DOXYOGENPOTS) Doxyfile_HOST_LL.in
	@$(DOXYGENBUILD) $(DOXYOGENPOTS) Doxyfile_DEVICES.in

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile | doxygen
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR_SPHINX)" "$(BUILDDIR_SPHINX)" $(SPHINXOPTS) $(O)

clean:
	rm -rf _build_*
	@$(SPHINXBUILD) -M clean "$(SOURCEDIR_SPHINX)" "$(BUILDDIR_SPHINX)" $(SPHINXOPTS) $(O)