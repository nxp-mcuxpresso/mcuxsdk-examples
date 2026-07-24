Hardware requirements
=====================
- USB Type-C cable
- MIMXRT2660-EVK board (on-board APS256XXN-OBx9 16-bit Xccela PSRAM on XSPI1)
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
APS256XXN identity: MR1=0x8D, MR2=0xDF (OK) (expect 0x8D/0xDF)
XSPI edma example started!
EDMA Command Read/Write data successfully at all address range !
~~~~~~~~~~~~~~~~~~~~~

Note
====
- The PSRAM controller/device bring-up (CLK_CFG, auto-DLL, Xccela LUT,
  X16 entry) is done by BOARD_Init16bitsPsRam() in board.c; DMA writes go
  through MAIN__EDMA3 channel 0 (XSPI1 TX-buffer-fill request), reads and
  the verify pass use the non-cacheable AHB window at 0x81800000.
