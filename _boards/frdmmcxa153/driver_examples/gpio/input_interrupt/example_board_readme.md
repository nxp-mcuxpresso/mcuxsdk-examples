Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA153 board
- Personal Computer

Board settings
============

Prepare the Demo
================
1. Connect a Type-C USB cable between the host PC and the MCU-Link port(J15) on the target board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 GPIO Driver example

 Press SW2 to turn on/off a LED 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
And you will detect voltage toggle on J1_12 of board.