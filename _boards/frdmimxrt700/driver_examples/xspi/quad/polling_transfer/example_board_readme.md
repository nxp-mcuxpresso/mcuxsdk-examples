Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT700 board
- Personal Computer

Board settings
==============
No special settings are required. XSPI0 is used, which connects to the on-board W25Q25PWZEIM
Quad SPI NOR flash (32MB).

The flash-ops object (`xspi_quad_flash_ops.c`) and the XSPI driver object are
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
XSPI QPI SDR (4-4-4) polling example started!
4-byte address mode entered.
QPI (4-4-4) mode enabled.
Flash vendor ID: 0xEF

--- Test size: 64 bytes ---
  IP read: PASS
  AHB read: PASS

--- Test size: 128 bytes ---
  IP read: PASS
  AHB read: PASS

--- Test size: 256 bytes ---
  IP read: PASS
  AHB read: PASS

XSPI QPI SDR (4-4-4) polling example finished.
Flash reset to default SPI mode.
~~~~~~~~~~~~~~~~~~~~~
