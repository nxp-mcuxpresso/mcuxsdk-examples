Hardware requirements
===================
- Mini USB cable
- USB to TTL converter
- FRDM-MCXC242 board
- Personal Computer

Board settings
============
Connect the USB to TTL converter to FRDM-MCXC242 board.
FRDM-MCXC242 UART pins:
- TX of USB2COM to J1-5
- RX of USB2COM to J1-7
- GND of USB2COM to J2-14

Prepare the Demo
===============
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Connect the USB to TTL converter to PC host.
3.  Open a serial terminal on PC for the USB to TTL converter with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Reset the SoC and run the project.

Running the demo
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
USART CMSIS interrupt example
Board receives 8 characters then sends them out
Now please input:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When you input 8 characters, system will echo it by UART and them would be seen on the OpenSDA terminal.

