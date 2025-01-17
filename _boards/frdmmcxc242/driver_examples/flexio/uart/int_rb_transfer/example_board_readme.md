Hardware requirements
===================
- USB-C cable
- FRDM-MCXC242 board
- USB-to-COM external
- Personal Computer

Board settings
============
The flexio_uart_interrupt_ring_buffer example is requires connecting the FLEXIO pins with the USB2COM pins
The connection should be set as following:
- J1-9, TX of USB2COM connected
- J1-11, RX of USB2COM connected
- J2-14, Ground of USB2COM connected

Prepare the Demo
===============
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
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
FLEXIO UART RX ring buffer example
Send back received data
Echo every 8 bytes
~~~~~~~~~~~~~~~~~~~~~
