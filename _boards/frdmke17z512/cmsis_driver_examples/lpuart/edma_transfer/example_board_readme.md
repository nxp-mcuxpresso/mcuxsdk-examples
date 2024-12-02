Hardware requirements
===================
- Micro USB cable
- FRDM-KE17Z512 board
- Personal Computer

Board settings
============
No special is needed.

Prepare the Demo
===============
1.  Connect a USB cable between the PC host and the MCU-LINK USB port on the board.
2.  Open a serial terminal on PC for MCU-LINK serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
===============
When the demo runs successfully, the log would be seen on the MCU-LINK terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~
USART EDMA example
Send back received data
Echo every 8 characters
~~~~~~~~~~~~~~~~~~~~~~~~~~~
