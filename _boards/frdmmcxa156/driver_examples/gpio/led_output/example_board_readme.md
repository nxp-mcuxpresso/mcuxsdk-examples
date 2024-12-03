Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA156 board
- Personal Computer

Board settings
============

Prepare the Demo
================
1. Connect a USB Type-C cable between the host PC and MB board J22.
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
The LED is blinking.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
And you will detect voltage toggle on J4.P21 of MB board.
