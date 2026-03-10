#!/bin/bash

# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0

docker exec chimera zsh -c "
	cd /app/chimera/chimera-sdk &&
cmake -D TARGET_PLATFORM=chimera-convolve \
			-D TOOLCHAIN_DIR=/app/install/llvm-18.1.4-pulp \
			-D PICOLIBC_DIR=/app/install/picolibc \
			-D HARDWARE_BACKEND=RTL \
			-D CHIMERA_UNIFIED_ELF=ON \
			-B build &&
	cmake --build build
"
