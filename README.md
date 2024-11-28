# CHIMERA SDK

Chimera-SDK is a bare-metal development platform for ASICs based on the [Chimera architecture](https://github.com/pulp-platform/chimera), a microcontroller SoC for multi-cluster, heterogeneous systems.

Chimera and Chimera-SDK are developed as part of the PULP project, a joint effort between ETH Zurich and the University of Bologna.

## License

Unless specified otherwise in the respective file headers, all code checked into this repository is made available under a permissive license. All software sources are licensed under Apache 2.0 (see LICENSE) or compatible licenses, with the exception of `scripts/run_clang_format.py`, which is licensed under the MIT license.

## Getting started

If you are working on an IIS system, you can use the preinstalled SEPP packages to add the correct versions of the RISC-V toolchain and questasim into your path. If you use bash, you might run
```
riscv bash
```

If you are not on an IIS system, please ensure a RISC-V toolchain and questa installation are available in your path by checking that the output of
```
which riscv32-unknown-elf-gcc
```
is not empty.

## LLVM Toolchain

The applications are built using the RISC-V LLVM toolchain. On IIS systems, users can use the pre-installed LLVM version 12.0.1.

Outside of IIS systems, you need to install LLVM version 12.0.1 or later to ensure compatibility. The exact command on IIS systems is:
```
cmake -DTARGET_PLATFORM=[YOURTARGETPLATFORM] -DTOOLCHAIN_DIR=/usr/pack/riscv-1.0-kgf/pulp-llvm-0.12.0/bin ..  && cmake --build .
```

The correct version of the toolchain can be verified by running
```
llvm-config --version
```

## Building the SDK

To build the SDK and all tests contained in the SDK, run

```
mkdir build && cd build
cmake -DTARGET_PLATFORM=[YOURTARGETPLATFORM]  ../
cmake --build .
```

where you should replace `[YOURTARGETPLATFORM]` by one of the platforms defined in `targets/CMakeLists.txt` under `AVAILABLE_TARGETS`.

The resulting binaries will be stored in `build/bin`, and can be used within the `chimera` repo as tests.

## CXX Formatting

To format all source files, run
```
python scripts/run_clang_format.py -ir hal/ targets/ tests/ driver/
```

Our CI uses llvm-12 for clang-format, so on IIS machines you may run
```
python scripts/run_clang_format.py -ir tests/ hal/ targets/ driver/ --clang-format-executable=/usr/pack/riscv-1.0-kgf/pulp-llvm-0.12.0/bin/clang-format

```

## Visual Studio Code Integration

To enable automatic configuration of the C/C++ extension and support for the integrated cMake build flow on the IIS workstations, add the following content to `.vscode/settings.json`:
```json
{
    "cmake.configureSettings": {
        "TOOLCHAIN_DIR": "/usr/pack/riscv-1.0-kgf/pulp-llvm-0.12.0/bin",
        "TARGET_PLATFORM": "chimera-convolve",
    },
    "cmake.environment": {
        "PATH": "/usr/pack/riscv-1.0-kgf/default/bin:${env:PATH}",
        "LD_LIBRARY_PATH": "/usr/pack/riscv-1.0-kgf/lib64:/usr/pack/riscv-1.0-kgf/lib64",
    }
}
```

