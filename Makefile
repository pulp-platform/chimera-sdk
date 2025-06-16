# Copyright 2025 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

# Authors: 
# - Philip Wiese <wiesep@iis.ee.ethz.ch>
# - Victor Jung <jungvi@iis.ee.ethz.ch>

ROOT_DIR := $(patsubst %/,%, $(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

INSTALL_DIR ?= ${ROOT_DIR}/install
TOOLCHAIN_DIR := ${ROOT_DIR}/toolchain

LLVM_INSTALL_DIR ?= ${INSTALL_DIR}/llvm
LLVM_CLANG_RT_RISCV_RV32IMC ?= ${LLVM_INSTALL_DIR}/lib/clang/15.0.0/lib/baremetal/rv32imc/libclang_rt.builtins-riscv32.a
GVSOC_INSTALL_DIR ?= ${ROOT_DIR}/${INSTALL_PREFIX}

LLVM_COMMIT_HASH ?= 1ccb97ef1789b8c574e3fcab0de674e11b189b96
GVSOC_COMMIT_HASH ?= ffba6ded9abf0a2c86d0b4096ec20a4392af96ad

CLANG_FORMAT_EXECUTABLE ?= clang-format

CMAKE ?= cmake

help:
	@echo "Usage: make <target>"
	@echo ""
	@echo "Available Targets:"
	@echo " - llvm: Install LLVM @ ${LLVM_INSTALL_DIR}"
	@echo " - gvsoc: Install GVSoC @ ${GVSOC_INSTALL_DIR}"
	@echo " - export-symbols: Print the list of symbols to export to run the SDK's tests"
	@echo " - format: Format all code"

format:
	@echo "Formatting code..."
	@python scripts/run_clang_format.py -ir tests/ hal/ targets/ drivers/ devices/ --clang-format-executable=$(CLANG_FORMAT_EXECUTABLE)
	@yapf -rip -e "install/" -e "toolchain/" .

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


${TOOLCHAIN_DIR}/llvm-project:
	mkdir -p ${TOOLCHAIN_DIR} && \
	cd ${TOOLCHAIN_DIR} && \
	git clone https://github.com/pulp-platform/llvm-project.git \
	 -b main && \
	cd ${TOOLCHAIN_DIR}/llvm-project && git checkout ${LLVM_COMMIT_HASH} && \
	git submodule update --init --recursive

${LLVM_INSTALL_DIR}: ${TOOLCHAIN_DIR}/llvm-project
	cd ${TOOLCHAIN_DIR}/llvm-project && \
	mkdir -p build && cd build && \
	${CMAKE} -G Ninja \
	-DCMAKE_INSTALL_PREFIX=${LLVM_INSTALL_DIR} \
	-DLLVM_ENABLE_PROJECTS="clang;lld" \
	-DLLVM_TARGETS_TO_BUILD="ARM;RISCV;host" \
	-DLLVM_BUILD_DOCS="0" \
	-DLLVM_ENABLE_BINDINGS="0" \
	-DLLVM_ENABLE_TERMINFO="0" \
	-DLLVM_OPTIMIZED_TABLEGEN=ON \
	-DLLVM_PARALLEL_LINK_JOBS=2 \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_C_COMPILER_LAUNCHER=ccache \
	-DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
	../llvm && \
	${CMAKE} --build . -j && \
	${CMAKE} --install .

llvm: ${LLVM_INSTALL_DIR}


${LLVM_CLANG_RT_RISCV_RV32IMC}: ${TOOLCHAIN_DIR}/llvm-project
	mkdir -p ${TOOLCHAIN_DIR} && \
	cd ${TOOLCHAIN_DIR}/llvm-project && mkdir -p build-compiler-rt-riscv-rv32imc \
	&& cd build-compiler-rt-riscv-rv32imc; \
	${CMAKE} ../compiler-rt \
	-DCMAKE_C_COMPILER_WORKS=1 \
	-DCMAKE_CXX_COMPILER_WORKS=1 \
	-DCMAKE_AR=${LLVM_INSTALL_DIR}/bin/llvm-ar \
	-DCMAKE_INSTALL_PREFIX=${LLVM_INSTALL_DIR}/lib/clang/15.0.0 \
	-DCMAKE_ASM_COMPILER_TARGET="riscv32-unknown-elf" \
	-DCMAKE_C_COMPILER=${LLVM_INSTALL_DIR}/bin/clang \
	-DCMAKE_ASM_COMPILER=${LLVM_INSTALL_DIR}/bin/clang \
	-DCMAKE_C_FLAGS="-mno-relax -march=rv32imc" \
	-DCMAKE_SYSTEM_NAME=baremetal \
	-DCMAKE_HOST_SYSTEM_NAME=baremetal \
	-DCMAKE_C_COMPILER_TARGET="riscv32-unknown-elf" \
	-DCMAKE_CXX_COMPILER_TARGET="riscv32-unknown-elf" \
	-DCMAKE_SIZEOF_VOID_P=4 \
	-DCMAKE_NM=${LLVM_INSTALL_DIR}/bin/llvm-nm \
	-DCMAKE_RANLIB=${LLVM_INSTALL_DIR}/bin/llvm-ranlib \
	-DCOMPILER_RT_BUILD_BUILTINS=ON \
	-DCOMPILER_RT_BUILD_LIBFUZZER=OFF \
	-DCOMPILER_RT_BUILD_MEMPROF=OFF \
	-DCOMPILER_RT_BUILD_PROFILE=OFF \
	-DCOMPILER_RT_BUILD_SANITIZERS=OFF \
	-DCOMPILER_RT_BUILD_XRAY=OFF \
	-DCOMPILER_RT_DEFAULT_TARGET_ONLY=ON \
	-DCOMPILER_RT_BAREMETAL_BUILD=ON \
	-DCOMPILER_RT_OS_DIR="baremetal/rv32imc" \
	-DLLVM_CONFIG_PATH=${LLVM_INSTALL_DIR}/bin/llvm-config && \
	${CMAKE} --build . -j && \
	${CMAKE} --install .

llvm-compiler-rt-riscv: ${LLVM_CLANG_RT_RISCV_RV32IMC}


.PHONY: format help export-symbols