# Copyright 2022 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Authors: Viviane Potocnik <vivianep@iis.ee.ethz.ch>

#!/usr/bin/env python3
import os
import subprocess
import argparse
import glob


def run_tool(cmd):
    """Run a command without printing it."""
    subprocess.run(cmd, check=True)


def find_ip_configs(opentitan_dir):
    """
    Recursively search for hjson files in the opentitan_dir that are likely
    IP configuration files. We only keep those files where the filename (without
    extension) is the same as the name of the parent directory.
    """
    pattern = os.path.join(opentitan_dir, "hw", "**", "data", "*.hjson")
    candidates = glob.glob(pattern, recursive=True)
    ip_configs = []
    for file in candidates:
        # Only include files like .../data/<ip_name>.hjson
        file_basename = os.path.splitext(os.path.basename(file))[0]
        parent_dir = os.path.basename(os.path.dirname(file))
        if file_basename == parent_dir:
            ip_configs.append((file_basename, file))
    return ip_configs


def main():
    # Compute absolute path to the OpenTitan repository.
    script_dir = os.path.dirname(__file__)
    opentitan_dir = os.path.abspath(
        os.path.join(script_dir,
                     "../../drivers/uart_opentitan/thirdparty/opentitan"))

    # Define paths to tools and key input files.
    regtool_py = os.path.join(opentitan_dir, "util/regtool.py")
    topgen_py = os.path.join(opentitan_dir, "util/topgen.py")
    dttool_py = os.path.join(opentitan_dir, "util/dttool.py")
    autogen_dif_py = os.path.join(opentitan_dir, "util/autogen_dif.py")
    dt_ext_py = os.path.join(opentitan_dir,
                             "hw/ip_templates/rstmgr/util/dt.py")
    top_hjson = os.path.join(opentitan_dir,
                             "hw/top_earlgrey/data/top_earlgrey.hjson")
    autogen_dif_out = os.path.join(opentitan_dir, "sw/device/lib/dif/autogen")

    # Parse command-line arguments.
    parser = argparse.ArgumentParser(
        description="Generate headers for embedded IPs for Chimera-SDK")
    parser.add_argument(
        "--output",
        type=str,
        required=True,
        help="Output directory for generated headers",
    )
    args = parser.parse_args()
    os.makedirs(args.output, exist_ok=True)

    # Automatically discover IP hjson configuration files.
    ip_autogen_configs = find_ip_configs(opentitan_dir)

    # Define the set of IPs for which register headers should be generated.
    reg_ips = {
        "uart",
        "adc_ctrl",
        "aes",
        "aon",
        "csrng",
        "alert_handler",
        "aon_timer",
        "clkmgr",
        "edn",
        "entropy_src",
        "flash_ctrl",
        "gpio",
        "hmac",
        "i2c",
        "keymgr",
        "keymgr_dpe",
        "kmac",
        "lc_ctrl",
        "otbn",
        "otp_ctrl",
        "pattgen",
        "pinmux",
        "pwm",
        "pwrmgr",
        "rom_ctrl",
        "rstmgr",
        "rv_core_ibex",
        "rv_plic",
        "rv_timer",
        "sensor_ctrl",
        "spi_device",
        "spi_host",
        "sram_ctrl",
        "sysrst_ctrl",
        "usbdev",
        "rv_dm",
    }

    # Process each IP: run regtool, dttool, and autogen_dif.
    for ip_name, ipcfg in ip_autogen_configs:
        if ip_name in reg_ips:
            reg_header = os.path.join(args.output, f"{ip_name}_regs.h")
            cmd = [regtool_py, "-D", "-o", reg_header, ipcfg]
            run_tool(cmd)

    # Generate top-level autogen HJSON using topgen.
    cmd_topgen = [topgen_py, "-t", top_hjson, "-o", args.output]
    run_tool(cmd_topgen)
    autogen_hjson = os.path.join(args.output,
                                 "data/autogen/top_earlgrey.gen.hjson")

    # Generate top-level DT files.
    cmd_dttool_top = [
        dttool_py,
        "--topgencfg",
        autogen_hjson,
        "--outdir",
        args.output,
        "--gen-top",
    ]
    run_tool(cmd_dttool_top)

    # Generate IP-level DT headers.
    for ip_name, ip_hjson in ip_autogen_configs:
        cmd = [
            dttool_py,
            "--topgencfg",
            autogen_hjson,
            "--outdir",
            args.output,
            "--gen-ip",
            "-i",
            ip_hjson,
        ]
        if ip_name == "rstmgr":
            ipconfig_file = os.path.join(
                opentitan_dir,
                "hw/top_earlgrey/ip_autogen/rstmgr/data/top_earlgrey_rstmgr.ipconfig.hjson",
            )
            cmd.extend(["--ipconfig", ipconfig_file, "--extension", dt_ext_py])
        run_tool(cmd)

    # Ensure the DIF autogen output directory exists.
    os.makedirs(autogen_dif_out, exist_ok=True)
    # Generate DIF autogen headers.
    for ip_name, ipcfg in ip_autogen_configs:
        cmd = [autogen_dif_py, "--ipcfg", ipcfg, "--outdir", autogen_dif_out]
        run_tool(cmd)


if __name__ == "__main__":
    main()
