Hardware requirements
=====================
- USB cable
- FRDM-IMXRT700 board
- Personal Computer

Board settings
==============
Remove SJ425, SJ426, SJ427, SJ428, SJ429 and populate SJ420, SJ421, SJ422, SJ423, SJ424.

XSPI1 NAND pin assignment:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
NAND Pin   RT700 Signal   PIO
/CS        XSPI1_SS0_N    PIO5_0
CLK        XSPI1_SCLK0    PIO5_10
IO0        XSPI1_DATA0    PIO5_1
IO1        XSPI1_DATA1    PIO5_2
IO2        XSPI1_DATA2    PIO5_3
IO3        XSPI1_DATA3    PIO5_4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the MCU-Link USB port on the target board.
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
When the demo runs successfully, the log would be seen on the terminal like:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
***NAND Flash Component Demo Start!***

***NAND Flash Initialization Start!***

***NAND Flash Initialization Success!***

***NAND Flash Erase The First Block Start!***

***NAND Flash Erase Check Start!***

***NAND Flash Erase block Success!***

***NAND Flash Page Program Start!***

***NAND Flash Page Read Start!***
.....
.....
.....

***NAND Flash Page Read/Write Success!***

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
