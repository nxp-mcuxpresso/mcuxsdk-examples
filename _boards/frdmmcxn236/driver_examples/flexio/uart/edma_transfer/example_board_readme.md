Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN236 board
- USB to Serial Converter
- Personal Computer

Board settings
============
The flexio_uart_polling example is requires connecting the FLEXIO pins with the USB2COM pins
The connection should be set as following:
- J8-18(P3_1), RX of USB2COM connected
- J8-17(P3_0), TX of USB2COM connected
- J8-2, Ground of USB2COM connected

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the EVK board J10.
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
When the demo runs successfully, the log would be seen on the UART Terminal port which connected to the USB2COM like:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Flexio uart edma example
Board receives 8 characters then sends them out
Now please input:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
