Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT700 board
- Personal Computer

Board settings
==============
No special settings are required. XSPI0 is used, which connects to the on-board W25Q25PWZEIM
Quad SPI NOR flash (32MB). EDMA (DMA0, channels 0/1) is used for page program transfers.

The flash-ops object (`xspi_quad_flash_edma_ops.c`) and the XSPI/EDMA driver objects are
placed in SRAM by the custom linker scripts so that erase/program operations do not stall
instruction fetch from the same flash.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see similar output in the terminal:
~~~~~~~~~~~~~~~~~~~~~
XSPI QPI SDR (4-4-4) EDMA example started!
4-byte address mode entered.
QPI (4-4-4) mode enabled.
Flash vendor ID: 0xEF
Erasing Serial NOR over XSPI...
Erase done.
Erase verify (EDMA read) - OK.
Page program done.
==> EDMA program+read: PASS.
==> AHB program+read: PASS.
XSPI QPI SDR (4-4-4) EDMA example finished.
Flash reset to default SPI mode.
~~~~~~~~~~~~~~~~~~~~~
