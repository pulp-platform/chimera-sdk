# Copyright 2025 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

adapter speed 8000
adapter driver ftdi
ftdi_vid_pid 0x0403 0x6014
ftdi_layout_init 0x00e8 0x60eb
ftdi_channel 0
set irlen 5

transport select jtag
telnet_port disabled
tcl_port disabled
gdb_port 3333
reset_config none

set _CHIPNAME riscv
jtag newtap $_CHIPNAME cpu -irlen ${irlen} -expected-id 0x1c5e5db3

set _TARGETNAME $_CHIPNAME.cpu
target create $_TARGETNAME riscv -chain-position $_TARGETNAME -coreid 0

gdb_report_data_abort enable
gdb_report_register_access_error enable

riscv set_reset_timeout_sec 120
riscv set_command_timeout_sec 120

# riscv set_prefer_sba off

# Exit when debugger detaches
# $_TARGETNAME configure -event gdb-detach {
#     echo "GDB detached; ending debugging session."
#     shutdown
# }

init
halt
echo "Ready for Remote Connections."