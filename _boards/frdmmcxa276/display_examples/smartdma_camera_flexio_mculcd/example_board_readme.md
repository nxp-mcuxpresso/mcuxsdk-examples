Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA276 board
- Personal Computer

Board settings
============
- Attach OV7670 to J9 camera interface(pin1 to pin18).
- Attach LCD-PAR-S035 low cost panel to J8. Set the switch SW1 on LCD-PAR-S035 to 010.
- Remove R73 and U4 if the board is using CMSIS DAP debug firmware, no need to rework if using J-Link.

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
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
When the example runs successfully, the image that the camera captures will show on the right side of the panel.