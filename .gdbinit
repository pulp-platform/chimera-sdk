# ---- Basics ----
set pagination off
set print pretty on
set disassemble-next-line on
set target-async on
set remotetimeout 120

set architecture riscv:rv32

# ---- Helpers ----
define oc
  target extended-remote localhost:3333
  monitor reset halt
  echo Connected and halted.\n
end

define restart
  # Set program counter to _start and continue
  set $pc = _start
  continue
end

# ---- Print a0 helpers ----
# When execution stops (e.g. on ebreak), detect and print a0
define hook-stop
  set $insn32 = *(unsigned int *)$pc
  set $insn16 = *(unsigned short *)$pc
  if $insn32 == 0x00100073 || $insn16 == 0x9002
    printf "[GDB] Return Value: 0x%x (%d)\n", $a0, $a0
  end
end

echo [GDB] Available Shortcuts:\n
echo   oc       : Connects to OpenOCD and halts the target\n
echo   restart  : Sets PC to _start and continues execution\n
echo \n