# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

ROOT_DIR := $(patsubst %/,%, $(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

INSTALL_DIR ?= /app/install
TOOLCHAIN_DIR := ${ROOT_DIR}/toolchain

GVSOC_INSTALL_DIR := ${INSTALL_DIR}/gvsoc

# LLVM 21.1.8
# LLVM_DIR := llvm-21.1.8
# LLVM_INSTALL_DIR := ${INSTALL_DIR}/${LLVM_DIR}
# LLVM_GIT_URL := https://github.com/llvm/llvm-project.git
# LLVM_COMMIT_HASH ?= 2078da43e25a4623cab2d0d60decddf709aaea28

# LLVM 18.1.4-pulp
LLVM_DIR := llvm-18.1.4-pulp
LLVM_INSTALL_DIR := ${INSTALL_DIR}/${LLVM_DIR}
LLVM_GIT_URL := git@iis-git.ee.ethz.ch:iis-compilers/llvm-project.git
LLVM_COMMIT_HASH ?= cc8682a5602aa2ea1907bafa21afe7ec041b9d22

# LLVM 15.0.0-pulp
# LLVM_DIR := llvm-15.0.0-pulp
# LLVM_INSTALL_DIR := ${INSTALL_DIR}/${LLVM_DIR}
# LLVM_GIT_URL := https://github.com/pulp-platform/llvm-project.git
# LLVM_COMMIT_HASH ?= 1ccb97ef1789b8c574e3fcab0de674e11b189b96

GVSOC_COMMIT_HASH ?= 68e835cd52c55e0fd467a1863b4701cf90478dc6

CLANG_FORMAT_EXECUTABLE ?= clang-format

CMAKE ?= cmake

.PHONY: help
help:
	@echo "Usage: make <target>"
	@echo ""
	@echo "Available Targets:"
	@echo " - llvm: Install LLVM @ ${LLVM_INSTALL_DIR}"
	@echo " - gvsoc: Install GVSoC @ ${GVSOC_INSTALL_DIR}"
	@echo " - export-symbols: Print the list of symbols to export to run the SDK's tests"
	@echo " - format: Format all code"
	@echo " - picolibc-multilib: Build picolibc for all RISC-V variants"
	@echo " - container: Build the chimera-sdk Docker image"

.PHONY: format
format:
	@echo "Formatting code..."
	@uv sync
	@uv run pre-commit run --all-files

.PHONY: export-symbols
export-symbols:
	@echo "Please export the following symbols:"
	@echo "GVSOC_HOME=${GVSOC_INSTALL_DIR}/gvsoc"

# -----------------------------------------------------------------------------
# GVSoC build
# -----------------------------------------------------------------------------

${TOOLCHAIN_DIR}/gvosc:
	mkdir -p ${TOOLCHAIN_DIR} && cd ${TOOLCHAIN_DIR} && \
	git clone https://github.com/Xeratec/gvsoc.git && \
	cd ${TOOLCHAIN_DIR}/gvsoc && git checkout ${GVSOC_COMMIT_HASH} && \
	git submodule update --init --recursive && \
	uv pip install --system -r core/requirements.txt && \
	uv pip install --system -r gapy/requirements.txt && \
	uv pip install --system setuptools==81.0.0 && \
	make all TARGETS=chimera INSTALLDIR=${GVSOC_INSTALL_DIR}

${GVSOC_INSTALL_DIR}: ${TOOLCHAIN_DIR}/gvosc

gvsoc: ${GVSOC_INSTALL_DIR}

# -----------------------------------------------------------------------------
# LLVM/Clang/LLD build (Ninja)
# -----------------------------------------------------------------------------
${TOOLCHAIN_DIR}/${LLVM_DIR}:
	mkdir -p ${TOOLCHAIN_DIR} && \
	cd ${TOOLCHAIN_DIR} && \
	git init ${LLVM_DIR} && \
	cd ${TOOLCHAIN_DIR}/${LLVM_DIR} && \
	git remote add origin ${LLVM_GIT_URL} && \
	git fetch --depth=1 origin ${LLVM_COMMIT_HASH} && \
	git checkout ${LLVM_COMMIT_HASH} && \
	git submodule update --init --recursive


LLVM_BUILD_DIR := $(TOOLCHAIN_DIR)/$(LLVM_DIR)/build-llvm
LLVM_STAMP     := $(LLVM_INSTALL_DIR)/.llvm-installed

# Use a stamp (or you can use $(LLVM_INSTALL_DIR)/bin/clang) so rebuilds trigger correctly.
$(LLVM_STAMP): $(TOOLCHAIN_DIR)/$(LLVM_DIR)
	@set -e; \
	cd $(TOOLCHAIN_DIR)/$(LLVM_DIR); \
	mkdir -p $(LLVM_BUILD_DIR); \
	cd $(LLVM_BUILD_DIR); \
	$(CMAKE) -G Ninja \
	  -DCMAKE_INSTALL_PREFIX=$(LLVM_INSTALL_DIR) \
	  -DLLVM_ENABLE_PROJECTS="clang;lld" \
	  -DLLVM_TARGETS_TO_BUILD="RISCV;host" \
	  -DLLVM_BUILD_DOCS=OFF \
	  -DLLVM_ENABLE_BINDINGS=OFF \
	  -DLLVM_ENABLE_TERMINFO=OFF \
	  -DLLVM_OPTIMIZED_TABLEGEN=ON \
	  -DLLVM_PARALLEL_LINK_JOBS=2 \
	  -DCMAKE_BUILD_TYPE=Release \
	  -DCMAKE_C_COMPILER_LAUNCHER=ccache \
	  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
	  ../llvm; \
	$(CMAKE) --build . -j; \
	$(CMAKE) --install .; \
	touch $(LLVM_STAMP)

llvm: $(LLVM_STAMP)

# Compute Clang resource dir dynamically (avoids hardcoding "21").
# NOTE: This is evaluated when referenced; it requires clang to be installed.
CLANG_RESOURCE_DIR = $(shell $(LLVM_INSTALL_DIR)/bin/clang -print-resource-dir)

# -----------------------------------------------------------------------------
# compiler-rt builtins (baremetal multilib variants)
# -----------------------------------------------------------------------------

CRT_COMMON_CMAKE_FLAGS = \
	-DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
	-DCMAKE_SYSTEM_NAME=Generic \
	-DCMAKE_AR=$(LLVM_INSTALL_DIR)/bin/llvm-ar \
	-DCMAKE_NM=$(LLVM_INSTALL_DIR)/bin/llvm-nm \
	-DCMAKE_RANLIB=$(LLVM_INSTALL_DIR)/bin/llvm-ranlib \
	-DCMAKE_LINKER=$(LLVM_INSTALL_DIR)/bin/ld.lld \
	-DCMAKE_C_COMPILER=$(LLVM_INSTALL_DIR)/bin/clang \
	-DCMAKE_ASM_COMPILER=$(LLVM_INSTALL_DIR)/bin/clang \
	-DCMAKE_CXX_COMPILER=$(LLVM_INSTALL_DIR)/bin/clang \
	-DLLVM_CONFIG_PATH=$(LLVM_INSTALL_DIR)/bin/llvm-config \
	-DCOMPILER_RT_BUILD_BUILTINS=ON \
	-DCOMPILER_RT_BUILD_LIBFUZZER=OFF \
	-DCOMPILER_RT_BUILD_MEMPROF=OFF \
	-DCOMPILER_RT_BUILD_PROFILE=OFF \
	-DCOMPILER_RT_BUILD_SANITIZERS=OFF \
	-DCOMPILER_RT_BUILD_XRAY=OFF \
	-DCOMPILER_RT_DEFAULT_TARGET_ONLY=ON \
	-DCOMPILER_RT_BAREMETAL_BUILD=ON

# Macro to generate one compiler-rt builtins build+install target.
# Args:
#  1: name (install subdir + build dir suffix), e.g. rv32imc
#  2: cmake processor (CMAKE_SYSTEM_PROCESSOR), e.g. riscv32
#  3: triple, e.g. riscv32-unknown-elf
#  4: -march value, e.g. rv32imc
#  5: -mabi value, e.g. ilp32
#  6: sizeof void* (4 or 8)
define MAKE_CRT_BUILTINS_TARGET

CRT_STAMP_$(1) := $(LLVM_INSTALL_DIR)/.compiler-rt-builtins-$(1)-installed
CRT_BUILD_DIR_$(1) := $(TOOLCHAIN_DIR)/$(LLVM_DIR)/build-compiler-rt-$(1)

$$(CRT_STAMP_$(1)): $(TOOLCHAIN_DIR)/$(LLVM_DIR) $(LLVM_STAMP)
	@set -e; \
	mkdir -p $$(CRT_BUILD_DIR_$(1)); \
	cd $$(CRT_BUILD_DIR_$(1)); \
	$(CMAKE) $(TOOLCHAIN_DIR)/$(LLVM_DIR)/compiler-rt \
	  $(CRT_COMMON_CMAKE_FLAGS) \
	  -DCMAKE_SYSTEM_PROCESSOR=$(2) \
	  -DCMAKE_ASM_COMPILER_TARGET="$(3)" \
	  -DCMAKE_C_COMPILER_TARGET="$(3)" \
	  -DCMAKE_CXX_COMPILER_TARGET="$(3)" \
	  -DCMAKE_SIZEOF_VOID_P=$(6) \
	  -DCMAKE_INSTALL_PREFIX="$$(CLANG_RESOURCE_DIR)" \
	  -DCOMPILER_RT_OS_DIR="baremetal/$(1)" \
	  -DCMAKE_C_FLAGS="-march=$(4) -mabi=$(5)"; \
	$(CMAKE) --build . -j; \
	$(CMAKE) --install .; \
	touch $$@

.PHONY: compiler-rt-$(1)
compiler-rt-$(1): $$(CRT_STAMP_$(1))

endef

# ---- Define your multilib variants here ----
# CVA6 Variants
$(eval $(call MAKE_CRT_BUILTINS_TARGET,rv32imc,riscv32,riscv32-unknown-elf,rv32imc,ilp32,4))
$(eval $(call MAKE_CRT_BUILTINS_TARGET,rv64imc,riscv64,riscv64-unknown-elf,rv64imc,lp64,8))
$(eval $(call MAKE_CRT_BUILTINS_TARGET,rv64imafdc,riscv64,riscv64-unknown-elf,rv64imafdc,lp64d,8))

# Snitch Cluster Variants
$(eval $(call MAKE_CRT_BUILTINS_TARGET,rv32ima,riscv32,riscv32-unknown-elf,rv32ima,ilp32,4))
$(eval $(call MAKE_CRT_BUILTINS_TARGET,rv32imafd,riscv32,riscv32-unknown-elf,rv32imafd,ilp32d,4))
# $(eval $(call MAKE_CRT_BUILTINS_TARGET,rv32ima_xdma,riscv32,riscv32-unknown-elf,rv32ima_xdma,ilp32,4))

# Convenience aggregate target
.PHONY: compiler-rt-riscv-multilib
compiler-rt-riscv-multilib: compiler-rt-rv32imc compiler-rt-rv32ima compiler-rt-rv64imc compiler-rt-rv32imafd compiler-rt-rv64imafdc #compiler-rt-rv32ima_xdma

# -----------------------------------------------------------------------------
# picolibc prebuilt (Meson + Ninja, one build per multilib variant)
# -----------------------------------------------------------------------------

PICOLIBC_GIT_URL     := https://github.com/picolibc/picolibc.git
PICOLIBC_GIT_TAG     := 1.8.11
PICOLIBC_SRC_DIR     ?= ${INSTALL_DIR}/picolibc-src
PICOLIBC_INSTALL_DIR := ${INSTALL_DIR}/picolibc

# Clone the official picolibc sources at the pinned tag.
PICOLIBC_CLONE_STAMP := $(PICOLIBC_SRC_DIR)/.picolibc-cloned

$(PICOLIBC_CLONE_STAMP):
	@if [ ! -f "$(PICOLIBC_SRC_DIR)/meson.build" ]; then \
		echo "[CHIMERA] Cloning picolibc $(PICOLIBC_GIT_TAG) into $(PICOLIBC_SRC_DIR)..."; \
		mkdir -p $(PICOLIBC_SRC_DIR); \
		git clone --depth 1 --branch $(PICOLIBC_GIT_TAG) $(PICOLIBC_GIT_URL) $(PICOLIBC_SRC_DIR); \
	fi
	touch $@

picolibc-clone: $(PICOLIBC_CLONE_STAMP)

# Macro to generate one picolibc build+install target.
# Args:
#  1: variant name (used in stamp/build-dir names), e.g. rv32im-ilp32
#  2: triple (--target), e.g. riscv32-unknown-elf
#  3: march value, e.g. rv32im
#  4: mabi value, e.g. ilp32
#  5: Meson cpu string, e.g. riscv32
#  6: Meson cpu_family string, e.g. riscv
define MAKE_PICOLIBC_TARGET

PICOLIBC_STAMP_$(1)      := $(PICOLIBC_INSTALL_DIR)/.picolibc-$(1)-installed
PICOLIBC_BUILD_DIR_$(1)  := $(PICOLIBC_SRC_DIR)/build-$(1)
PICOLIBC_CROSS_FILE_$(1) := $(PICOLIBC_SRC_DIR)/cross-$(1).txt

$$(PICOLIBC_CROSS_FILE_$(1)): $(PICOLIBC_CLONE_STAMP)
	@mkdir -p $(PICOLIBC_SRC_DIR)
	@printf '[binaries]\n'                                                    > $$@
	@printf "c     = '$(LLVM_INSTALL_DIR)/bin/clang'\n"                     >> $$@
	@printf "ar    = '$(LLVM_INSTALL_DIR)/bin/llvm-ar'\n"                   >> $$@
	@printf "strip = '$(LLVM_INSTALL_DIR)/bin/llvm-strip'\n"                >> $$@
	@printf '\n[host_machine]\n'                                             >> $$@
	@printf "system     = 'none'\n"                                         >> $$@
	@printf "cpu_family = '$(6)'\n"                                         >> $$@
	@printf "cpu        = '$(5)'\n"                                         >> $$@
	@printf "endian     = 'little'\n"                                       >> $$@
	@printf '\n[built-in options]\n'                                        >> $$@
	@printf "c_args      = ['--target=$(2)', '-nostdlib', '-ggdb',\n"       >> $$@
	@printf "               '-gdwarf-4', '-gstrict-dwarf',\n"               >> $$@
	@printf "               '-march=$(3)', '-mabi=$(4)']\n"                 >> $$@
	@printf "c_link_args = ['--target=$(2)', '-nostdlib', '-fno-common',\n" >> $$@
	@printf "               '-Wl,-z,noexecstack', '-fuse-ld=lld',\n"       >> $$@
	@printf "               '-march=$(3)', '-mabi=$(4)']\n"                 >> $$@

$$(PICOLIBC_STAMP_$(1)): $$(PICOLIBC_CROSS_FILE_$(1))
	@mkdir -p $$(PICOLIBC_BUILD_DIR_$(1)) $(PICOLIBC_INSTALL_DIR)
	meson setup \
		$$(PICOLIBC_BUILD_DIR_$(1)) $(PICOLIBC_SRC_DIR) \
		--cross-file $$(PICOLIBC_CROSS_FILE_$(1)) \
		-D multilib-list=$(3)/$(4) \
		--prefix $(PICOLIBC_INSTALL_DIR) \
		--default-library=static \
		--wipe
	ninja -C $$(PICOLIBC_BUILD_DIR_$(1))
	ninja -C $$(PICOLIBC_BUILD_DIR_$(1)) install
	ninja -C $$(PICOLIBC_BUILD_DIR_$(1)) clean
	touch $$@

.PHONY: picolibc-$(1)
picolibc-$(1): $$(PICOLIBC_STAMP_$(1))

endef

# ---- Define picolibc multilib variants ----
$(eval $(call MAKE_PICOLIBC_TARGET,rv32im-ilp32,riscv32-unknown-elf,rv32im,ilp32,riscv32,riscv))
$(eval $(call MAKE_PICOLIBC_TARGET,rv32imafd-ilp32d,riscv32-unknown-elf,rv32imafd,ilp32d,riscv32,riscv))
$(eval $(call MAKE_PICOLIBC_TARGET,rv64imafdc-lp64d,riscv64-unknown-elf,rv64imafdc,lp64d,riscv64,riscv))


.PHONY: picolibc-multilib
picolibc-multilib: picolibc-rv32im-ilp32 picolibc-rv32imafd-ilp32d picolibc-rv64imafdc-lp64d

# -----------------------------------------------------------------------------
# Docker container
# -----------------------------------------------------------------------------

.PHONY: container
container:
	$(MAKE) -C container chimera

# -----------------------------------------------------------------------------
# Documentation (Sphinx + Doxygen)
# -----------------------------------------------------------------------------

.PHONY: docs
docs:
	@uv sync
	cd docs && uv run $(MAKE) html

# Reroute all docs-* targets to the docs Makefile, which handles Sphinx and Doxygen.
docs-%:
	@uv sync
	cd docs && uv run $(MAKE) $*


