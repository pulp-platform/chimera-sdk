#!/usr/bin/env python3
import os
import subprocess
import argparse

# Compute the absolute path to the OpenTitan repository.
OPENTITON_DIR = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "../../drivers/uart_opentitan/thirdparty/opentitan")
)

# Paths to tools
REGTOOL_PY     = os.path.join(OPENTITON_DIR, "util/regtool.py")
TOPGEN_PY      = os.path.join(OPENTITON_DIR, "util/topgen.py")
DTTOOL_PY      = os.path.join(OPENTITON_DIR, "util/dttool.py")
AUTOGEN_DIF_PY = os.path.join(OPENTITON_DIR, "util/autogen_dif.py")
# The dt extension file for rstmgr:
DT_EXT_PY      = os.path.join(OPENTITON_DIR, "hw/ip_templates/rstmgr/util/dt.py")

# Other input file
TOP_HJSON = os.path.join(OPENTITON_DIR, "hw/top_earlgrey/data/top_earlgrey.hjson")

# Output directory for DIF autogen headers (fixed in the OpenTitan repo)
AUTOGEN_DIF_OUT = os.path.join(OPENTITON_DIR, "sw/device/lib/dif/autogen")

# Command-line argument parsing:
parser = argparse.ArgumentParser(
    description="Generate headers for embedded IPs for Chimera-SDK"
)
parser.add_argument("--output", type=str, required=True, help="Output directory for generated headers")
args = parser.parse_args()

# Ensure output directory exists.
os.makedirs(args.output, exist_ok=True)

# Define the list of IPs and their HJSON file paths.
ip_autogen_configs = [
    ("uart", os.path.join(OPENTITON_DIR, "hw/ip/uart/data/uart.hjson")),
    ("adc_ctrl", os.path.join(OPENTITON_DIR, "hw/ip/adc_ctrl/data/adc_ctrl.hjson")),
    ("aes", os.path.join(OPENTITON_DIR, "hw/ip/aes/data/aes.hjson")),
    ("aon_timer", os.path.join(OPENTITON_DIR, "hw/ip/aon_timer/data/aon_timer.hjson")),
    ("csrng", os.path.join(OPENTITON_DIR, "hw/ip/csrng/data/csrng.hjson")),
    ("rv_dm", os.path.join(OPENTITON_DIR, "hw/ip/rv_dm/data/rv_dm.hjson")),
    ("edn", os.path.join(OPENTITON_DIR, "hw/ip/edn/data/edn.hjson")),
    ("entropy_src", os.path.join(OPENTITON_DIR, "hw/ip/entropy_src/data/entropy_src.hjson")),
    ("hmac", os.path.join(OPENTITON_DIR, "hw/ip/hmac/data/hmac.hjson")),
    ("i2c", os.path.join(OPENTITON_DIR, "hw/ip/i2c/data/i2c.hjson")),
    ("keymgr", os.path.join(OPENTITON_DIR, "hw/ip/keymgr/data/keymgr.hjson")),
    # ("keymgr_dpe", os.path.join(OPENTITON_DIR, "hw/ip/keymgr_dpe/data/keymgr_dpe.hjson")),
    ("kmac", os.path.join(OPENTITON_DIR, "hw/ip/kmac/data/kmac.hjson")),
    ("lc_ctrl", os.path.join(OPENTITON_DIR, "hw/ip/lc_ctrl/data/lc_ctrl.hjson")),
    ("otbn", os.path.join(OPENTITON_DIR, "hw/ip/otbn/data/otbn.hjson")),
    ("pattgen", os.path.join(OPENTITON_DIR, "hw/ip/pattgen/data/pattgen.hjson")),
    ("pwm", os.path.join(OPENTITON_DIR, "hw/ip/pwm/data/pwm.hjson")),
    ("rom_ctrl", os.path.join(OPENTITON_DIR, "hw/ip/rom_ctrl/data/rom_ctrl.hjson")),
    ("rv_core_ibex", os.path.join(OPENTITON_DIR, "hw/ip/rv_core_ibex/data/rv_core_ibex.hjson")),
    ("rv_timer", os.path.join(OPENTITON_DIR, "hw/ip/rv_timer/data/rv_timer.hjson")),
    ("spi_device", os.path.join(OPENTITON_DIR, "hw/ip/spi_device/data/spi_device.hjson")),
    ("spi_host", os.path.join(OPENTITON_DIR, "hw/ip/spi_host/data/spi_host.hjson")),
    ("sram_ctrl", os.path.join(OPENTITON_DIR, "hw/ip/sram_ctrl/data/sram_ctrl.hjson")),
    ("sysrst_ctrl", os.path.join(OPENTITON_DIR, "hw/ip/sysrst_ctrl/data/sysrst_ctrl.hjson")),
    ("usbdev", os.path.join(OPENTITON_DIR, "hw/ip/usbdev/data/usbdev.hjson")),
    ("sensor_ctrl", os.path.join(OPENTITON_DIR, "hw/top_earlgrey/ip/sensor_ctrl/data/sensor_ctrl.hjson")),
    ("alert_handler", os.path.join(OPENTITON_DIR, "hw/top_earlgrey/ip_autogen/alert_handler/data/alert_handler.hjson")),
    ("clkmgr", os.path.join(OPENTITON_DIR, "hw/top_earlgrey/ip_autogen/clkmgr/data/clkmgr.hjson")),
    ("flash_ctrl", os.path.join(OPENTITON_DIR, "hw/top_earlgrey/ip_autogen/flash_ctrl/data/flash_ctrl.hjson")),
    ("gpio", os.path.join(OPENTITON_DIR, "hw/top_earlgrey/ip_autogen/gpio/data/gpio.hjson")),
    ("otp_ctrl", os.path.join(OPENTITON_DIR, "hw/top_earlgrey/ip_autogen/otp_ctrl/data/otp_ctrl.hjson")),
    ("pinmux", os.path.join(OPENTITON_DIR, "hw/top_earlgrey/ip_autogen/pinmux/data/pinmux.hjson")),
    ("pwrmgr", os.path.join(OPENTITON_DIR, "hw/top_earlgrey/ip_autogen/pwrmgr/data/pwrmgr.hjson")),
    ("rstmgr", os.path.join(OPENTITON_DIR, "hw/top_earlgrey/ip_autogen/rstmgr/data/rstmgr.hjson")),
    ("rv_plic", os.path.join(OPENTITON_DIR, "hw/top_earlgrey/ip_autogen/rv_plic/data/rv_plic.hjson")),
]

# Build the list of "-i" arguments for dttool from the IP list.
dttool_ip_args = []
for ip_name, hjson in ip_autogen_configs:
    if ip_name == "keymgr_dpe" or ip_name == "dma":
        print(f"Exempting {ip_name} from dttool")
        continue
    dttool_ip_args.extend(["-i", hjson])

# Build additional dttool arguments for ipconfig files.
# For rstmgr, add the corresponding ipconfig file.
dttool_ipconfig_args = []
for ip_name, _ in ip_autogen_configs:
    if ip_name == "rstmgr":
        ipconfig_file = os.path.join(
            OPENTITON_DIR,
            "hw/top_earlgrey/ip_autogen/rstmgr/data/top_earlgrey_rstmgr.ipconfig.hjson"
        )
        dttool_ipconfig_args.extend(["--ipconfig", ipconfig_file])
        break  # Assuming only one rstmgr entry

# Generate registers headers for IPs.
for ip_name, ipcfg in ip_autogen_configs:
    if ip_name in ("uart", "adc_ctrl", "aes", "aon", "csrng", "alert_handler",
                   "aon_timer", "clkmgr", "edn", "entropy_src", "flash_ctrl", "gpio",
                   "hmac", "i2c", "keymgr", "keymgr_dpe", "kmac", "lc_ctrl", "otbn", "otp_ctrl",
                   "pattgen", "pinmux", "pwm", "pwrmgr", "rom_ctrl", "rstmgr",
                   "rv_core_ibex", "rv_plic", "rv_timer", "sensor_ctrl", "spi_device",
                   "spi_host", "sram_ctrl", "sysrst_ctrl", "usbdev", "rv_dm"):
        reg_header = os.path.join(args.output, f"{ip_name}_regs.h")
        cmd = [REGTOOL_PY, "-D", "-o", reg_header, ipcfg]
        print(f"Running regtool for {ip_name}: {' '.join(cmd)}")
        subprocess.run(cmd, check=True)

# Generate top-level autogen HJSON using topgen.
cmd_topgen = [TOPGEN_PY, "-t", TOP_HJSON, "-o", args.output]
print(f"Running topgen: {' '.join(cmd_topgen)}")
subprocess.run(cmd_topgen, check=True)

# The topgen-generated autogen config is assumed to be in the following location.
AUTOGEN_HJSON = os.path.join(args.output, "data/autogen/top_earlgrey.gen.hjson")

# Generate top-level DT files.
cmd_dttool_top = [DTTOOL_PY,
                  "--topgencfg", AUTOGEN_HJSON,
                  "--outdir", args.output,
                  "--gen-top"]
print(f"Running dttool for top-level: {' '.join(cmd_dttool_top)}")
subprocess.run(cmd_dttool_top, check=True)

# Now, generate IP-level DT headers separately for each IP.
for ip_name, ip_hjson in ip_autogen_configs:
    cmd = [DTTOOL_PY,
           "--topgencfg", AUTOGEN_HJSON,
           "--outdir", args.output,
           "--gen-ip",
           "-i", ip_hjson]
    # If this IP requires an ipconfig file, add it.
    if ip_name in ["rstmgr"]:
        cmd.extend(["--ipconfig", os.path.join(
            OPENTITON_DIR,
            "hw/top_earlgrey/ip_autogen/rstmgr/data/top_earlgrey_rstmgr.ipconfig.hjson"
        )])
        # Also add the extension flag.
        cmd.extend(["--extension", DT_EXT_PY])
    print(f"Running dttool for IP {ip_name}: {' '.join(cmd)}")
    subprocess.run(cmd, check=True)

# Ensure the DIF autogen output directory exists.
os.makedirs(AUTOGEN_DIF_OUT, exist_ok=True)

# Generate DIF autogen headers for each IP.
for ip_name, ipcfg in ip_autogen_configs:
    cmd = [AUTOGEN_DIF_PY, "--ipcfg", ipcfg, "--outdir", AUTOGEN_DIF_OUT]
    print(f"Running autogen_dif for {ip_name}: {' '.join(cmd)}")
    subprocess.run(cmd, check=True)

# List the IPs processed.
ip_list = [ip_name for ip_name, _ in ip_autogen_configs]
print(f"Generated headers for IPs: {', '.join(ip_list)}")
