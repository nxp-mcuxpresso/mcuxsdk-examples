Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXC444 board
- Personal Computer

Board settings
============
No special is needed.

Prepare the Demo
===============
1.  Connect a type-c USB cable between the host PC and the MCU-Link USB port (J13) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
5.  A multimeter may be used to measure the DAC output voltage.

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

DAC buffer interrupt Example.

DAC Buffer Information
          Buffer index max  : 1
Press any key in terminal to trigger the buffer ...

Buffer Index  0: ReadPointerTopPositionEvent
Buffer Index  1: ReadPointerBottomPositionEvent

User can take a look at how the pointer is moved by triggered, then user can also measure the DAC output
pin J6-2 to check responding voltage.
