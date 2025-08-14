Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW72 Board
- Personal Computer

Board settings
============
No special is needed.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the FRDM board J10.
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
And you will detect voltage toggle on J1 Pin 6.
If the FRDM board is used then the blue led will blink.
