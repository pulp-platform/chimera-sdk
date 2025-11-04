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

## Formatting and Linting

We provide the [pre-commit](https://pre-commit.com) configuration file which you can use to install github hooks that execute the formatting commands on your changes.

We recommend that you setup a virtual environment and install the required dependencies using the following commands:
```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
pip install -r requirements-docs.txt
```

The configuration sets the default stage for all the hooks to `pre-push` so to install the git hooks run:
```bash
pre-commit install --hook-type pre-push
```
The hooks will run before each push, making sure the pushed code can pass linting checks and not fail the CI on linting.

If you change your mind and don't want the git hooks:
```bash
pre-commit uninstall
```

You can also run the hooks manually on all files using:
```bash
pre-commit run --all-files
# Or use the Makefile target
make format
```

**The Makefile is only used for utility purposes and not by the build system.!**


## License
All licenses used in this repository are listed under the `LICENSES` folder. Unless specified otherwise in the respective file headers, all code checked into this repository is made available under a permissive license.
- Most software sources and tool scripts are licensed under the [Apache 2.0 license](https://opensource.org/licenses/Apache-2.0).
- Some files in the `scripts` directory are licensed under the [MIT license](https://opensource.org/license/mit).
- Markdown, JSON, text files, pictures, PDFs, are licensed under the [Creative Commons Attribution 4.0 International](https://creativecommons.org/licenses/by/4.0) license (CC BY 4.0).

To extract license information for all files, you can use the [reuse tool](https://reuse.software/) and by running `reuse spdx` in the root directory of this repository.

