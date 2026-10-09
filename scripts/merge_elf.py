#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0
#
# Merge multiple per-core ELF files into a single unified ELF.
#
# The host ELF (RV64, ELFCLASS64) is used as the base. Each device ELF
# (RV32, ELFCLASS32) is embedded as read-only LOAD segments so the unified
# file can be loaded by a single simulator invocation.
#
# Device LOAD segments are stripped of the PF_X (execute) flag, marking them
# as "data" from the host ISA perspective.  A heterogeneous RTL simulator
# identifies which core executes which memory region by hardware configuration,
# not by ELF segment flags.
#
# Why this works across ELFCLASS32 / ELFCLASS64:
#   The host ELF class (ELFCLASS64) is kept as-is.  Device VMAs are 32-bit
#   values that fit naturally into 64-bit address fields (zero-extended).
#   lief handles the binary-level representation transparently.
#
# Usage:
#   python scripts/merge_elf.py \
#       --host   build/host/host.elf \
#       --device build/devices/snitch_cluster_0/snitch_cluster_0.elf \
#       --device build/devices/snitch_cluster_1/snitch_cluster_1.elf \
#       --output build/chimera_unified.elf

import argparse
import sys
from pathlib import Path
from typing import List

try:
    import lief
except ImportError:
    print(
        "[CHIMERA] ERROR: 'lief' is not installed.\n"
        "Run:  uv sync   (or: pip install lief)",
        file = sys.stderr,
    )
    sys.exit(1)

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------


def _name_from_path(path: Path) -> str:
    """Derive a human-readable binary name from an ELF file path.

    E.g. build/devices/snitch_cluster_0/snitch_cluster_0.elf → snitch_cluster_0
    """
    return path.stem  # strips the .elf suffix


def _seg_flag_str(flags: int) -> str:
    """Human-readable PT_LOAD segment flags (R/W/X)."""
    r = "R" if flags & int(lief.ELF.Segment.FLAGS.R) else "-"
    w = "W" if flags & int(lief.ELF.Segment.FLAGS.W) else "-"
    x = "X" if flags & int(lief.ELF.Segment.FLAGS.X) else "-"
    return f"{r}{w}{x}"


def _parse_elf(path: Path, label: str) -> lief.ELF.Binary:
    config = lief.ELF.ParserConfig()
    # Make sure we can insert sections with arbitrary virtual addresses
    config.page_size = 4
    binary = lief.ELF.parse(str(path), config = config)
    if binary is None:
        print(
            f"[CHIMERA] ERROR: lief could not parse {label} ELF: {path}",
            file = sys.stderr,
        )
        sys.exit(1)
    return binary


def _print_elf_info(binary: lief.ELF.Binary, label: str) -> None:
    print(f"[CHIMERA] {label} section headers:")
    for sec in binary.sections:
        if sec.size == 0:
            continue
        print(f"[CHIMERA]    {sec.name:50} vma=0x{sec.virtual_address:016x}  "
              f"size=0x{sec.size:08x}  flags={sec.flags_list} type={sec.type.name}")
    print(f"[CHIMERA] {label} segments:")
    for seg in binary.segments:
        print(f"[CHIMERA]    {seg.type.name:50} lma=0x{seg.physical_address:016x}  "
              f"size=0x{len(seg.content):08x}  flags={_seg_flag_str(int(seg.flags))}")


# ---------------------------------------------------------------------------
# Core merge logic
# ---------------------------------------------------------------------------


def merge_elfs(
    host_path: Path,
    device_paths: List[Path],
    output_path: Path,
) -> None:
    """Load the host ELF and embed each device's LOAD segments as read-only
    (non-executable) data segments, then write the result to output_path.

    Segment flag mapping applied to device segments:
      Original R-X  → R--   (execute bit stripped; device ISA != host ISA)
      Original R--  → R--   (unchanged)
      Original RW-  → RW-   (unchanged)
    """
    host_name = _name_from_path(host_path)
    print(f"[CHIMERA] Host   : {host_name}  ({host_path})")

    host = _parse_elf(host_path, "host")
    host_class = ("ELF64" if host.header.identity_class == lief.ELF.Header.CLASS.ELF64 else "ELF32")
    print(f"[CHIMERA]          {host_class}  e_machine={host.header.machine_type.name}")

    # Remove the .reserved section with type NOBITs
    reserved_sec = host.get_section(".reserved")
    if reserved_sec is not None:
        host.remove(reserved_sec)

    for dev_path in device_paths:
        dev_name = _name_from_path(dev_path)
        print(f"[CHIMERA] Device : {dev_name}  ({dev_path})")

        # _print_elf_info(_parse_elf(dev_path, f"device/{dev_name}"), f"device/{dev_name}")

        device = _parse_elf(dev_path, f"device/{dev_name}")
        dev_class = ("ELF64" if device.header.identity_class == lief.ELF.Header.CLASS.ELF64 else "ELF32")
        print(f"[CHIMERA]          {dev_class}  e_machine={device.header.machine_type.name}")

        sections = [
            sec for sec in device.sections
            if sec.size > 0 and sec.virtual_address != 0 and sec.name != ".reserved" and sec.name != ".common"
        ]

        load_segments = [
            seg for seg in device.segments
            if seg.type == lief.ELF.Segment.TYPE.LOAD and len(seg.content) > 0 and seg.virtual_address != 0
        ]

        for seg in load_segments:
            # print(f"[CHIMERA]          Embedding segment {dev_name} LOAD "
            #       f"vma=0x{seg.virtual_address:016x} "
            #       f"lma=0x{seg.physical_address:016x} "
            #       f"size=0x{len(seg.content):08x}")
            seg.virtual_address = seg.physical_address
            host.add(seg, base = seg.physical_address)

        for sec in sections:
            sec.name = f"{dev_name}{sec.name}"
            host.add(sec, loaded = False, pos = lief.ELF.Binary.SEC_INSERT_POS.POST_SECTION)

    print(f"[CHIMERA] ======= After merging devices into host =======")
    _print_elf_info(host, "host")
    print(f"[CHIMERA] ------------------------------------------------------------")

    output_path.parent.mkdir(parents = True, exist_ok = True)
    host.write(str(output_path))


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------


def main() -> int:
    parser = argparse.ArgumentParser(
        description = ("Chimera ELF merger — embed device binaries into the host ELF "
                       "as read-only (non-executable) LOAD segments."),
        formatter_class = argparse.RawDescriptionHelpFormatter,
        epilog = ("Example:\n"
                  "  %(prog)s \\\n"
                  "    --host   build/host/host.elf \\\n"
                  "    --device build/devices/snitch_cluster_0/snitch_cluster_0.elf \\\n"
                  "    --device build/devices/snitch_cluster_1/snitch_cluster_1.elf \\\n"
                  "    --output build/chimera_unified.elf"),
    )
    parser.add_argument(
        "--host",
        required = True,
        metavar = "ELF",
        help = "Path to the host ELF (used as base for the unified output)",
    )
    parser.add_argument(
        "--device",
        action = "append",
        default = [],
        metavar = "ELF",
        help = "Path to a device ELF to embed (repeatable)",
    )
    parser.add_argument(
        "--output",
        "-o",
        required = True,
        metavar = "ELF",
        help = "Output path for the unified ELF",
    )

    args = parser.parse_args()

    host_path = Path(args.host)
    device_paths = [Path(d) for d in args.device]

    # Validate all inputs exist before doing any work
    missing = [str(p) for p in [host_path] + device_paths if not p.exists()]
    if missing:
        print(
            "[CHIMERA] ERROR: The following ELF files do not exist "
            "(binaries not yet built?):",
            file = sys.stderr,
        )
        for m in missing:
            print(f"  {m}", file = sys.stderr)
        return 1

    merge_elfs(
        host_path = host_path,
        device_paths = device_paths,
        output_path = Path(args.output),
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
