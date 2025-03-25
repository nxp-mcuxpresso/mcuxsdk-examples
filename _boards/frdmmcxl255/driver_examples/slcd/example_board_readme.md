Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
============
Make sure you have solder bridges SJ1 and SJ2 set to
- 2-1 for the main core (cm33)
- 2-3 for AON core (cm0+)

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
You will se some breathtakingly beautiful characters precisely printed on
the screen of onboard display.
