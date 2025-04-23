# Copyright 2025 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

# Authors:
# - Philip Wiese <wiesep@iis.ee.ethz.ch>
# - Victor Jung <jungvi@iis.ee.ethz.ch>
# - Viviane Potocnik <vivianep@iis.ee.ethz.ch>

ROOT_DIR := $(patsubst %/,%, $(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

INSTALL_PREFIX ?= install

GVSOC_INSTALL_DIR ?= ${ROOT_DIR}/${INSTALL_PREFIX}
GVSOC_COMMIT_HASH ?= ffba6ded9abf0a2c86d0b4096ec20a4392af96ad

CLANG_FORMAT_EXECUTABLE ?= clang-format

help:
	@echo "Usage: make <target>"
	@echo ""
	@echo "Available Targets:"
	@echo " - gvsoc: Install GVSoC @ ${GVSOC_INSTALL_DIR}"
	@echo " - export-symbols: Print the list of symbols to export to run the SDK's tests"
	@echo " - format: Format all code"

format:
	@echo "Formatting code..."
	@python scripts/run_clang_format.py -ir tests/ hal/ targets/ drivers/ devices/ --clang-format-executable=$(CLANG_FORMAT_EXECUTABLE)
	@echo "Formatting Python files with YAPF..."
	@find . \
	    -type f \
	    -name "*.py" \
	    ! -path "*/thirdparty/*" \
	    ! -name ".*" \
	    ! -path "*/.*/*" \
	    ! -path "*/build/*" \
	    -print0 \
	    | xargs -0 -I{} sh -c 'echo "Formatting Python file: {}" && python -m yapf -i "{}"'

export-symbols:
	@echo "Please export the following symbols:"
	@echo "GVSOC_HOME=${GVSOC_INSTALL_DIR}/gvsoc"

gvsoc:
	mkdir -p ${GVSOC_INSTALL_DIR} && cd ${GVSOC_INSTALL_DIR} && \
	git clone git@github.com:gvsoc/gvsoc.git && \
	cd ${GVSOC_INSTALL_DIR}/gvsoc && git checkout ${GVSOC_COMMIT_HASH} && \
	git submodule update --init --recursive && \
	pip install -r core/requirements.txt && \
	pip install -r gapy/requirements.txt && \
	CXX=g++-11.2.0 CC=gcc-11.2.0 CMAKE=cmake-3.18.1 make all TARGETS=chimera

.PHONY: format help export-symbols gvsoc