Hardware requirements
===================
- Type-C USB cable
- FRDM-MCXC444 board
- Personal Computer
- USB to Com Converter

Board settings
============
Connect pin:
- RX of USB2COM to J1_6
- TX of USB2COM to J2-4
- GND of USB2COM to J2-14

Prepare the Demo
===============
1.  Connect USB to Com Converter between the PC host and the port on the board.
2.  Open a serial terminal on PC for serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
===============
When the demo runs successfully, the log would be seen on the UART Terminal port which connected to the USB2COM like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~
USART DMA example
Send back received data
Echo every 8 characters
~~~~~~~~~~~~~~~~~~~~~~~~~~~

When you input 8 characters, system will echo it by UART and them would be seen on the terminal.
