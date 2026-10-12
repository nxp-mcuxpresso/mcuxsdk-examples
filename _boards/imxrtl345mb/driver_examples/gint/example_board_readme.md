Hardware requirements
=====================
- Type-C USB cable
- IMXRTL345MB board
- Personal Computer

Board settings
==============

Prepare the Demo
================
1.  Connect a Type-C USB cable between the host PC and the MCU-Link USB port on the target board.
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
1.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

The following lines are printed to the serial terminal when the demo program is executed.

Group GPIO input interrupt example

TODO: GINT0 and GINT1 events are configured

Press corresponding switches to generate events

2. This example configures "Group GPIO interrupt 0"" to be invoked when SW1 switch is pressed by the user.
   The interrupt callback prints "GINT0 event detected" message on the serial terminal. ""Group GPIO 
   interrupt 1"" is configured to be invoked when SW3 is pressed by the user.
   The interrupt callback prints "GINT1 event detected" message on the serial terminal.
