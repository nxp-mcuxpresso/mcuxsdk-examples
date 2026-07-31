Hardware requirements
=====================
- USB Type-C cable
- MIMXRT2660-EVK board (on-board APS512XXN-OBx9 16-bit Xccela PSRAM on XSPI1)
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect the USB cable between the PC host and the MCU-Link USB port on the board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
XSPI example started!
AHB Command Read/Write data successfully at all address range !
IP Command Read/Write data successfully at all address range !
~~~~~~~~~~~~~~~~~~~~~

Note
====
- The example fully re-configures the PSRAM through the XSPI driver at the
  device maximum of 250 MHz: global reset, mode-register setup (variable
  read latency LC=10, write latency WLC=9), X16 entry (MR8[6]) - all in the
  board-specific xspi_psram_ops.c (the shared ops file implements HyperBus,
  which this Xccela device does not use).
- RBX is not available at 250 MHz, so bursts are kept inside the 2 KB device
  row by the controller (1 KB AHB prefetch alignment, 256 B write page split,
  1 KB-aligned IP accesses).
- On the PSRAM-resident targets (psram/psram_txt/xspi_nor_psram) the example
  tests a 32 KB window carved out of NCACHE by the example-private linker
  scripts, skips Global Reset (memory content preserved), and runs the
  re-configuration code from ITCM/DTCM.
- The AHB accesses run through the XSPI1 direct (LLC-bypass) window at
  0x80000000, covered with a non-cacheable MPU region by BOARD_InitHardware.
