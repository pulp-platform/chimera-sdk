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

## Folder Structure and Targets
The SDK is organized into the following main folders:

```
chimera-sdk
├─ cmake                                # CMake configuration and utility files
├─ devices                              # Device-specific code with potential external dependencies such as runtime libraries
│ ├─ snitch_cluster
│ │ ├─ trampoline_snitchCluster.c       # Trampoline code to offload functions to the cluster
│ │ └─ snitch_runtime (external repo)   # External runtime library for the Snitch cluster
│ └─ <more devices>
├─ drivers                              # Host drivers for peripherals and clusters
│ ├─ cluster
│ └─ <more drivers>
├─ hal                                  # Hardware Abstraction Layer (HAL)
├─ scripts                              # Utility scripts
├─ targets                              # Target definitions such as memory map, register definitions and build configurations
│ ├─ chimera-open
│ └─ <more targets>
├─ tests                                # Test applications for each target
| ├─ chimera-open    
| └─ <more targets>
└─ CMakelists.txt                       # Top level CMakeLists.txt
```

### Targets:
The SDK supports multiple targets, each with a different configuration. The available targets are defined in `targets/CMakeLists.txt` under `AVAILABLE_TARGETS`. The following targets are currently available:

- `chimera-convolve`: Target for the CONVOLVE project.
- `chimera-host`: Target with a single host core without clusters.
- `chimera-open`: Default target with multiple clusters.

### Tests
Each target has a corresponding folder in the `tests` directory. The tests are organized by category, such as `host` or `snitchCluster`. Each category contains multiple tests. The tests are separated into host and cluster code.

```
tests
├─ chimera-open                   # Test applications for the chimera-open target
| ├─ <category>                   # Test categories (what part of the system is tested, e.g host, snitchCluster)
| │ ├─ test_1               
| │ │ ├─ host                     # Host code
| │ │ └─ snitchCluster            # Snitch sluster code
| │ └─ <more tests>
| └─ <more categories>
└─ <more targets>
```

## CXX Formatting

To format all source files, run
```
python scripts/run_clang_format.py -ir hal/ targets/ tests/ drivers/
```

Our CI uses llvm-12 for clang-format, so on IIS machines you may run
```
python scripts/run_clang_format.py -ir tests/ hal/ targets/ drivers/ --clang-format-executable=/usr/pack/riscv-1.0-kgf/pulp-llvm-0.12.0/bin/clang-format

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
If you are not on an IIS system, you need to adjust the paths according to your local installation.

## Technical Details

### Mixed ISA Compilation
The current approach compiles all code for both the host and cluster cores into a single library. This requires precise handling to ensure compatibility between the different instruction set architectures (ISAs) and application binary interfaces (ABIs).
This requires careful handling to avoid invalid instructions caused by mismatched ISAs between the host and cluster cores. Hence, we define four CMake variables,`ABI`, `ISA_HOST`, and `ISA_CLUSTER_SNITCH`, to specify the appropriate ISA for each core type. The ABI has to be identical to ensure correct function calls.
Furthermore, the tests are split into `src_host` and `src_cluster` directories to clearly separate code executed on the host and cluster cores. 

### cMake Build Flow
All runtime functions executed by the host core are compiled into a dedicated `runtime_host` static library and the cluster code into `runtime_cluster_<type>` (e.g. `runtime_cluster_snitch`). Additionally, the HAL layer is compiled into the `hal_host` static libary.
The final binary is seperated into two object libaries, one for the host and one for the cluster core. The host object library is linked with the `runtime_host` and `hal_host` libraries, while the cluster object library is linked with the `runtime_cluster_<type>` library. The final binary is then linked from the two object libraries.

### Warning
Special attention is required for functions that execute before the cluster core is fully initialized, such as the trampoline function and interrupt handlers. At this stage, critical resources like the stack, global pointer, and thread pointer are not yet configured. Consequently, the compiler must not generate code that allocates stack frames. To address this, such functions are implemented as naked functions, which prevent the compiler from adding prologues or epilogues that rely on stack operations.

**It is recommended to always check the generated assembly code to ensure that the correct instructions are generated for the target core!**

