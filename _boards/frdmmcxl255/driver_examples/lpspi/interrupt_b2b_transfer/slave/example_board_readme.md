Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
==============

LPSPI:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            MASTER(SPI1)        connect to      SLAVE(SPI1)
Pin Name    Board Location      Pin Name        Board Location
SOUT        J2-8                SIN             J2-10
SIN         J2-10               SOUT            J2-8
SCK         J2-12               SCK             J2-12
PCS0        J2-6                PCS1            J2-6
GND         J2-14               GND             J2-14
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Make sure you have solder bridges SJ1 and SJ2 set to
- 2-1 for the main core (cm33)

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port (J16) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

