Hardware requirements
===================
- Micro USB cable
- MCX-N9XX-EVK board
- Personal Computer
- 3.5" TFT LCD module by NXP (P/N PAR-LCD-S035)

Board settings
============
Attach the LCD shield to the EVK board.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J5) on the board
2.  Build the project.
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
================
If this example runs correctly, the sample GUI is displayed.

Note
================
**NOTE**: Revision A0 chip can't use SmartDMA for FlexIO display. Use the function
`Chip_GetVersion` to check silicon revision. For A0 chip, `BOARD_USE_FLEXIO_SMARTDMA`
must be set to 0, for A1 chip, `BOARD_USE_FLEXIO_SMARTDMA` can be set to 1
for better performance.
