# CHIMERA SDK

Chimera-SDK is a bare-metal development platform for ASICs based on the [Chimera architecture](https://github.com/pulp-platform/chimera), a microcontroller SoC for multi-cluster, heterogeneous systems.

Chimera and Chimera-SDK are developed as part of the PULP project, a joint effort between ETH Zurich and the University of Bologna.

## License

Unless specified otherwise in the respective file headers, all code checked into this repository is made available under a permissive license. All software sources are licensed under Apache 2.0 (see LICENSE) or compatible licenses, with the exception of `scripts/run_clang_format.py`, which is licensed under the MIT license.

## Documentation
All revelevant documentation can be found in the `docs` folder and is hosted on GitHub Pages.
Access the documentation on 
- [Master Branch](https://pulp-platform.github.io/chimera-sdk/)
- [Devel Branch](https://pulp-platform.github.io/chimera-sdk/branch/devel)

The documentation for a specific branch can be accessed via `https://pulp-platform.github.io/chimera-sdk/branch/<branch>`

## Contributing
### CXX Formatting
To format all source files, run
```
python scripts/run_clang_format.py -ir hal/ targets/ tests/ drivers/
```

Our CI uses llvm-12 for clang-format, so on IIS machines you may run
```
python scripts/run_clang_format.py -ir tests/ hal/ targets/ drivers/ --clang-format-executable=/usr/pack/riscv-1.0-kgf/pulp-llvm-0.12.0/bin/clang-format
```

### Python Formatting
To format all python files, run
```bash
python -m yapf -rip .
```

