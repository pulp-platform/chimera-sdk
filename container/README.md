# chimera-sdk Docker Container

This directory contains the `Dockerfile` and `Makefile` to build the `chimera-sdk` toolchain
container.  The image ships with:

- **LLVM 18.1.4-pulp** — PULP-flavoured RISC-V LLVM (clang, lld, llvm-nm, …)
- **compiler-rt builtins** — baremetal multilib variants: `rv32imc`, `rv32ima`, `rv32imafd`,
  `rv64imc`, `rv64imafdc`
- **picolibc 1.8.11** — multilib variants: `rv32im/ilp32`, `rv32imafd/ilp32d`, `rv64imafdc/lp64d`
- ccache, cmake, zsh, Oh My ZSH

The pre-built image is published at `ghcr.io/pulp-platform/chimera:latest`.

## Prerequisites

- Docker with BuildKit support (`DOCKER_BUILDKIT=1` or `docker buildx`)
- An SSH key with access to `iis-git.ee.ethz.ch` (required to clone the internal LLVM source)

## Building

From the **repository root**:

```bash
# Using the Container Makefile directly
make -C container chimera

# Or from the root Makefile
make container

# Custom image name or SSH key
make -C container chimera \
    CHIMERA_IMAGE=ghcr.io/my-org/chimera:devel \
    SSH_PRIVATE_KEY=~/.ssh/id_rsa
```

## Using the Container

```bash
docker run -it --rm \
    -v $(pwd):/app/chimera \
    ghcr.io/pulp-platform/chimera:latest \
    zsh

# Inside the container:
cd /app/chimera
cmake -D TARGET_PLATFORM=chimera-open \
      -D TOOLCHAIN_DIR=/app/install/llvm-18.1.4-pulp \
      -D PICOLIBC_DIR=/app/install/picolibc \
      -D HARDWARE_BACKEND=RTL \
      -D CHIMERA_UNIFIED_ELF=ON \
      -B build
cmake --build build -j
```

## Rebuilding Individual Tools

Because the build tree is preserved inside the image, individual tools can be rebuilt
by running `make` inside a container:

```bash
docker run --rm \
    --mount=type=ssh \
    -v ccache:/ccache \
    ghcr.io/pulp-platform/chimera:latest \
    make llvm
```
