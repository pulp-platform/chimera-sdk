#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
# SPDX-License-Identifier: Apache-2.0
#
# Post-build memory-map printer and section-overlap checker.
#
# Parses one `llvm-objdump -h` output file per binary, prints a memory map
# sorted by VMA, and checks for unintended overlaps between loaded (TEXT/DATA)
# sections of different binaries.
#
# BSS / NOLOAD sections (e.g. the shared .common region) are displayed but
# excluded from overlap checks because they are intentionally mapped at the
# same physical address on all cores.
#
# This is a Python port of cmake/scripts/CheckSectionOverlaps.cmake.
#
# Usage:
#   python scripts/check_section_overlaps.py \
#       --binary <name> <sections_file> \
#       --binary <name> <sections_file> \
#       ...

import argparse
import re
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import List

# ---------------------------------------------------------------------------
# Data model
# ---------------------------------------------------------------------------


@dataclass
class Section:
    binary: str  # display name of the owning binary
    name: str  # ELF section name (e.g. ".text", ".data")
    vma_start: int  # VMA start address (inclusive)
    vma_end: int  # VMA end address   (exclusive, = start + size)
    lma_start: int  # LMA start address (inclusive)
    lma_end: int  # LMA end address   (exclusive, = start + size)
    sec_type: str  # type string from objdump: TEXT, DATA, BSS, …


# ---------------------------------------------------------------------------
# objdump -h section parser
# ---------------------------------------------------------------------------

# llvm-objdump -h section line format:
#   <spaces> <idx> <name> <size_hex> <vma_hex> [<lma_hex>] <TYPE>
#
# Example (32-bit ELF):
#   0 .text    0000018c 80000000 TEXT
# Example (64-bit ELF):
#   0 .text    000001bc 0000000080000000 TEXT
#
# The VMA width differs between 32- and 64-bit ELFs; the regex accepts both.
_SECTION_RE = re.compile(
    r'^\s+\d+\s+'  # leading whitespace + index
    r'(\S+)\s+'  # (1) section name
    r'([0-9a-fA-F]+)\s+'  # (2) size  (hex)
    r'([0-9a-fA-F]+)\s+'  # (3) VMA   (hex)
    r'(?:([0-9a-fA-F]+)\s+)?'  # (4) optional LMA (hex)
    r'([A-Z]+)',  # (5) type  (TEXT / DATA / BSS / …)
    re.MULTILINE,
)


def parse_sections_file(path: Path, binary_name: str) -> List[Section]:
    """Parse one llvm-objdump -h output file; return a list of Section objects."""
    if not path.exists():
        print(
            f"[CHIMERA] WARNING: sections file not found "
            f"(binary not yet built?): {path}",
            file = sys.stderr,
        )
        return []

    sections: List[Section] = []
    for m in _SECTION_RE.finditer(path.read_text()):
        sec_name = m.group(1)
        size = int(m.group(2), 16)
        vma = int(m.group(3), 16)
        lma_str = m.group(4)
        lma = int(lma_str, 16) if lma_str is not None else vma
        sec_type = m.group(5)

        # Skip empty sections and metadata sections (VMA == 0 → debug/reloc)
        if vma == 0:
            continue

        sections.append(
            Section(
                binary = binary_name,
                name = sec_name,
                vma_start = vma,
                vma_end = vma + size,
                lma_start = lma,
                lma_end = lma + size,
                sec_type = sec_type,
            ))

    return sections


# ---------------------------------------------------------------------------
# Memory-map printer
# ---------------------------------------------------------------------------


def print_memory_map(all_sections: List[Section], verbose = False) -> None:
    """Print a memory map table sorted by VMA."""
    print("[CHIMERA] Checking for binaries %s" % ", ".join(sorted(set(s.binary for s in all_sections))))
    print("[CHIMERA] ===================================================================")
    print("[CHIMERA]  Memory Map  (all non-empty sections, sorted by VMA)")
    print("[CHIMERA] ===================================================================")
    print(
        "[CHIMERA]  VMA Start           VMA End             LMA Start           LMA End             Size        Type  Binary / Section"
    )
    print(
        "[CHIMERA] ---------------------------------------------------------------------------------------------------------------"
    )

    for sec in all_sections:
        size = sec.vma_end - sec.vma_start
        vma_start_hex = f"0x{sec.vma_start:016x}"
        vma_end_hex = f"0x{sec.vma_end:016x}"
        lma_start_hex = f"0x{sec.lma_start:016x}"
        lma_end_hex = f"0x{sec.lma_end:016x}"
        size_hex = f"0x{size:08x}"
        tag = "" if sec.sec_type in ("TEXT", "DATA") else "  [shared/NOLOAD]"

        if not verbose and size == 0:
            continue  # skip empty loaded sections in non-verbose mode
        print(f"[CHIMERA]  {vma_start_hex}  {vma_end_hex}  {lma_start_hex}  {lma_end_hex}  {size_hex:>10s}"
              f"  {sec.sec_type:<4s}  {sec.binary} / {sec.name}{tag}")

    print("[CHIMERA] ===================================================================")


# ---------------------------------------------------------------------------
# Overlap detector
# ---------------------------------------------------------------------------


def check_overlaps(loaded: List[Section], addr_space: str) -> bool:
    """Check all pairs of loaded (TEXT/DATA) sections from *different* binaries.

    Two ranges [s1, e1) and [s2, e2) overlap iff  s1 < e2  AND  s2 < e1.

    Returns True if at least one overlap was found, False otherwise.
    """
    found = False
    n = len(loaded)

    if addr_space == "VMA":
        start_attr = "vma_start"
        end_attr = "vma_end"
    elif addr_space == "LMA":
        start_attr = "lma_start"
        end_attr = "lma_end"
    else:
        raise ValueError(f"Unknown address space: {addr_space}")

    for i in range(n):
        a = loaded[i]
        if addr_space == "VMA" and getattr(a, start_attr) == 0x18000000:
            continue
        for j in range(i + 1, n):
            b = loaded[j]

            # Sections within the same binary never conflict with each other
            if a.binary == b.binary:
                continue

            a_start = getattr(a, start_attr)
            a_end = getattr(a, end_attr)
            b_start = getattr(b, start_attr)
            b_end = getattr(b, end_attr)

            if a_start < b_end and b_start < a_end:
                print(
                    f"[CHIMERA] OVERLAP ({addr_space}): {a.binary} / {a.name} "
                    f"[0x{a_start:x}, 0x{a_end:x}) overlaps "
                    f"{b.binary} / {b.name} [0x{b_start:x}, 0x{b_end:x})",
                    file = sys.stderr,
                )
                found = True

    return found


# ---------------------------------------------------------------------------
# Shared-section size consistency checker
# ---------------------------------------------------------------------------


def check_shared_section_sizes(all_sections: List[Section]) -> bool:
    """For sections with the same name that appear in multiple binaries at the
    same VMA start address (shared / NOLOAD sections), verify they have equal
    sizes.  A size mismatch indicates that the host and device compiled
    different struct layouts for the shared .common region.

    Returns True if at least one mismatch was found, False otherwise.
    """
    from itertools import combinations

    # Only check non-loaded sections (BSS / NOLOAD — e.g. .common)
    # Ignore sections called reserved
    sections = [x for x in all_sections if x.name != ".reserved"]

    by_name: dict = {}
    for s in sections:
        by_name.setdefault(s.name, []).append(s)

    found = False
    for name, secs in by_name.items():
        for a, b in combinations(secs, 2):
            if a.binary == b.binary:
                continue
            if a.vma_start != b.vma_start:
                continue  # Different VMA → not an intentionally shared section
            size_a = a.vma_end - a.vma_start
            size_b = b.vma_end - b.vma_start
            if size_a != size_b:
                print(
                    f"[CHIMERA] SIZE MISMATCH: shared section '{name}' has "
                    f"size 0x{size_a:x} in '{a.binary}' but "
                    f"0x{size_b:x} in '{b.binary}' — "
                    f"ensure both binaries compile the same shared.c",
                    file = sys.stderr,
                )
                found = True

    return found


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------


def main() -> int:
    parser = argparse.ArgumentParser(
        description = ("Chimera section-overlap checker — "
                       "Python port of cmake/scripts/CheckSectionOverlaps.cmake"),
        formatter_class = argparse.RawDescriptionHelpFormatter,
        epilog = ("Example:\n"
                  "  %(prog)s \\\n"
                  "    --binary build/devices/snitch_cluster_0/snitch_cluster_0.sections \\\n"
                  "    --binary build/devices/snitch_cluster_1/snitch_cluster_1.sections \\\n"
                  "    --binary build/host/host.sections"),
    )
    parser.add_argument(
        "--binary",
        "-b",
        action = "append",
        required = True,
        help = "Binary name and its llvm-objdump -h sections file (repeatable)",
    )
    parser.add_argument(
        "--verbose",
        "-v",
        action = "store_true",
        help = "Also print non-loaded sections (e.g. BSS, NOLOAD) in the memory map",
    )
    args = parser.parse_args()

    all_sections: List[Section] = []
    for sections_file in args.binary:
        name = sections_file.split("/")[-1].split(".")[0]
        all_sections.extend(parse_sections_file(Path(sections_file), name))

    if not all_sections:
        print("[CHIMERA] No non-empty sections found.")
        return 0

    # Sort by VMA start, then LMA start; use (binary, name) as tiebreaker for stable output
    all_sections.sort(key = lambda s: (s.lma_start, s.vma_start, s.binary, s.name))

    print_memory_map(all_sections, verbose = args.verbose)

    # Overlap check is limited to TEXT and DATA sections
    loaded = [s for s in all_sections if s.sec_type in ("TEXT", "DATA")]
    found_overlap_vma = check_overlaps(loaded, addr_space = "VMA")
    found_overlap_lma = check_overlaps(loaded, addr_space = "LMA")
    found_overlap = found_overlap_vma or found_overlap_lma

    found_size_mismatch = check_shared_section_sizes(all_sections)

    if found_overlap or found_size_mismatch:
        print(
            "[CHIMERA] ERROR: Section overlaps or shared-section size mismatches "
            "detected — see warnings above.\n"
            "Verify the placement chain in cmake/ChimeraUtils.cmake and "
            "each device's __device_end symbol and linker AT/LMA usage.",
            file = sys.stderr,
        )
        return 1

    print("[CHIMERA] => No section overlaps detected. Layout is clean.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
