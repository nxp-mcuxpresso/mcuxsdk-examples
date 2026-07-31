Hardware requirements
=====================
- USB-C cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
No special settings are required. The example drives the on-board W25H512NWEAM
QSPI NOR flash on XSPI0 (the boot flash) in Quad I/O (1-4-4) mode.

Prepare the Demo
================
1. Connect the USB-C cable to the debug USB port on the board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Build the debug (RAM) target and download the program to the target board
   with a debugger. The example erases and programs a test sector at the 1 MB
   offset of the boot flash, so it must run from RAM, not XIP.
4. Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the
terminal (one test block per size):

~~~~~~~~~~~~~~~~~~~~~
XSPI Quad I/O EDMA example started!
4-byte address mode entered.
Quad I/O mode enabled.
Flash vendor ID: 0xEF

--- Test: 64 bytes ---
  IP read verify: PASS
  AHB read verify: PASS
...
XSPI Quad I/O EDMA example finished.
Flash reset to default SPI mode.
~~~~~~~~~~~~~~~~~~~~~
