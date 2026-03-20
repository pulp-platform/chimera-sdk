# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
#
# SPDX-License-Identifier: Apache-2.0

# =============================================================================
# chimera-sdk toolchain container
#
# Includes: LLVM 18.1.4-pulp, compiler-rt multilib, picolibc multilib,
#           ccache, zsh (cypher theme), Oh My ZSH, gdb-multiarch.
#
# The LLVM source lives on an internal SSH server — pass your SSH agent:
#
#   DOCKER_BUILDKIT=1 docker build --ssh default -t chimera-sdk .
#
# To rebuild individual tools without reinstalling system packages, run make
# inside the container (the toolchain/ build tree is preserved):
#
#   docker run --rm -v ccache:/ccache chimera-sdk make llvm
# =============================================================================

FROM ubuntu:22.04

ARG TARGETPLATFORM
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Etc/UTC
ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8
ENV CC="ccache gcc"
ENV CXX="ccache g++"
ENV CCACHE_DIR=/ccache
ENV INSTALL_DIR=/app/install
ENV TOOLCHAIN_DIR=/app/install/llvm-18.1.4-pulp
ENV PICOLIBC_DIR=/app/install/picolibc
ENV LLVM_INSTALL_DIR=/app/install/llvm-18.1.4-pulp
ENV PATH="/app/install/llvm-18.1.4-pulp/bin:${PATH}"

WORKDIR /app/build

# Install all dependencies (system cmake 3.22 satisfies the project minimum of 3.22)
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    ccache \
    ninja-build \
    cmake \
    pkg-config \
    python3 \
    doxygen \
    python3-dev \
    python-is-python3 \
    curl \
    wget \
    git \
    git-lfs \
    openssh-client \
    ca-certificates \
    gdb-multiarch \
    nano \
    zsh && \
    rm -rf /var/lib/apt/lists/*

# Install uv and meson (meson is required by the picolibc Makefile target)
COPY --from=ghcr.io/astral-sh/uv:latest /uv /usr/local/bin/uv
RUN uv pip install --system meson

# Install Oh My ZSH with cypher theme
RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended && \
    sed -i 's/ZSH_THEME="robbyrussell"/ZSH_THEME="cypher"/' /root/.zshrc

COPY Makefile ./

# Build LLVM, compiler-rt, and picolibc via the project Makefile
RUN --mount=type=ssh \
    --mount=type=cache,target=/ccache \
    mkdir -p /root/.ssh && \
    ssh-keyscan iis-git.ee.ethz.ch >> /root/.ssh/known_hosts 2>/dev/null; \
    ccache -z && \
    make llvm && \
    make compiler-rt-riscv-multilib && \
    make picolibc-multilib && \
    ccache -s

# Build LLVM, compiler-rt, and picolibc via the project Makefile
RUN --mount=type=ssh \
    --mount=type=cache,target=/ccache \
    mkdir -p /root/.ssh && \
    ssh-keyscan iis-git.ee.ethz.ch >> /root/.ssh/known_hosts 2>/dev/null; \
    ccache -z && \
    make gvsoc && \
    ccache -s

WORKDIR /app
RUN rm -rf /app/build