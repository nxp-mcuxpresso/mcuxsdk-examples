Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW23 Board
- Personal Computer

Board settings
============
Use the default jumper settings.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the FRDM board J10.
2.  Open a serial terminal with the following settings
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build and download the application
4.  Start a debug session or press the reset button

Running the demo
================
When the demo runs successfully, the terminal displays similar information like the following:
~~~~~~~~~~~~~~~~~~

CDOG Peripheral Driver Example

CDOG IRQ Reached
* Miscompare fault occured *

intruction timer:   fffc5
intruction timer:   c615e
intruction timer:   8c34e
intruction timer:   5251a
intruction timer:   18703
......
CDOG IRQ Reached
* Timeout fault occured *

End of example
~~~~~~~~~~~~~~~~~~
Note:
To keep the program running correctly, it is recommended to perform a power on reset (POR) after loading the application.
SW reset does not clear pending fault flags.
