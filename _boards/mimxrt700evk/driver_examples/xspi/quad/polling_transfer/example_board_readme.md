Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Board settings
==============
Removed R396, R397, R400, R402. Populated R386, R694, R695, R701, R707, R708.

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
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see similar output in the terminal:
~~~~~~~~~~~~~~~~~~~~~
XSPI Quad I/O polling example started!
4-byte address mode entered.
Quad I/O mode enabled.
Flash vendor ID: 0xEF

--- Test: 64 bytes ---
  IP read verify: PASS
  AHB read verify: PASS

--- Test: 128 bytes ---
  IP read verify: PASS
  AHB read verify: PASS

--- Test: 256 bytes ---
  IP read verify: PASS
  AHB read verify: PASS

--- Test: 512 bytes ---
  IP read verify: PASS
  AHB read verify: PASS

XSPI Quad I/O polling example finished.
Flash reset to default SPI mode.
~~~~~~~~~~~~~~~~~~~~~
