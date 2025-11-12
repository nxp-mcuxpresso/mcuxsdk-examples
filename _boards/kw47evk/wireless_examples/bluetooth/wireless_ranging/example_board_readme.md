Toolchain supported
===================
- IAR Embedded Workbench (IDE version details are in the Release Notes)
- MCUXpresso IDE (IDE version details are in the Release Notes)
- GCC ARM Embedded (IDE version details are in the Release Notes)

Hardware requirements
=====================
- Type-C USB cable
- KW47-EVK Board
- Personal Computer

Board settings
==============
It is recommended to start the evaluation of this ranging system with a cabled configuration.
Once the system operation is properly understood, the evaluation can be pursued in more challenging environments (with multi-path, with interference).
To do so, a small hardware modification has to be done on the PCB (on EVK boards, the capacitor C3 has to be rotated 90 degrees to C4 position)
to bypass the printed antenna and connect the RF path to external SMA connector.
In order to perform radiated testing, connect an SMA-mounted antenna on each board instead of a cable.

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for the detected serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Press the reset button.
5.  One board will act as the ranging service client (the board that will initiate the measurement request).

Run the following commands:

```
communication role {central | peripheral}
range
```
6.  Repeat the steps above to prepare the second board for the measurement (choose the other available role in step 5).

Running the demo
================
Refer to "CS Wireless Ranging User Guide" documentation for detailed operation instructions.
