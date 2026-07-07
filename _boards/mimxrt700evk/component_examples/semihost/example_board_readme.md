Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer
- A debug probe that supports semihosting (the on-board MCU-Link, or a
  stand-alone J-Link / LinkServer probe)

Board settings
==============
No special board settings are required. Semihosting does not use the UART, so no
serial terminal is needed for the demo log.

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the MCU-Link on the board.
2.  Build the project (core `cm33_core0`) and download it to the target board.
3.  Enable semihosting in the debugger before running:
    - J-Link (GDB): `monitor semihosting enable` and, for batch capture,
      `monitor semihosting IOClient 2`.
    - LinkServer / MCUXpresso IDE: enable the semihost console in the launch
      configuration.
4.  Launch the debugger to begin running the demo.

Running the Demo
================
The demo creates a file named `semihost.txt` in the debugger's working directory
(the host machine, not the target), writes a line to it, reads it back, and
verifies the contents. The status appears on the debugger's semihosting console:
~~~~~~~~~~~~~~~~~~~~~
semihost: console + host file I/O demo
semihost: wrote "semihost.txt" (39 bytes) to the debugger host
semihost: read back 39 bytes, contents match
semihost: done
~~~~~~~~~~~~~~~~~~~~~
After running, `semihost.txt` is present on the host in the debugger's working
directory. On J-Link the target then exits cleanly via SYS_EXIT; on LinkServer,
if SYS_EXIT is ignored the target continues in an idle loop without faulting.
