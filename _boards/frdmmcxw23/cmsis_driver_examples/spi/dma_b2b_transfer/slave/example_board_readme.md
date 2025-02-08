Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW23 Board
- Personal Computer

Board settings
==============
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER_BOARD           CONNECTS TO         SLAVE_BOARD
Pin Name      Board Location            Pin Name      Board Location 
SPI_SSEL1     J1-5                      SPI_SSEL1     J1-5          
SPI_MOSI      J4-2                      SPI_MOSI      J4-2          
SPI_MISO      J4-1                      SPI_MISO      J4-1          
SPI_SCK       J4-6                      SPI_SCK       J4-6          
GND           J3-7                      GND           J3-7          
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Use the default jumper settings.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the FRDM board J10.
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
The following message shows in the terminal if the example runs successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CMSIS SPI board2board DMA example -- Slave transfer.


 Slave example is running...


 Slave receive:
      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
     10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
     20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
     30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
Succeed!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
