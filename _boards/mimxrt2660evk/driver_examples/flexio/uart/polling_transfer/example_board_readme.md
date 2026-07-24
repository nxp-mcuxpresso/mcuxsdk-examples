Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
============
- HSP_FLEXIO1_FXIO_D20 (PIO3_20, J34-12), RX of USB2COM connected
- HSP_FLEXIO1_FXIO_D21 (PIO3_21, J34-8 ), TX of USB2COM connected
- Ground connection

Remove the jumper on J53 2-3 and short J53 1-2.

Prepare the Demo
===============
1.  Connect the USB2COM Converter to the host PC 
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the UART Terminal port which connected to the USB2COM like:

~~~~~~~~~~~~~~~~~~~~~
Flexio uart polling example
Board will send back received characters
~~~~~~~~~~~~~~~~~~~~~
