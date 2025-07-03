Hardware requirements
=====================
- Type-C USB cable
- MCXW72-LOC Board
- Personal Computer

Board settings
============
To make the example work, connections needed to be as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER(LPSPI0)              connect to      SLAVE(FlexIO SPI)
Pin Name   Board Location                   Pin Name    Board Location
SOUT       J1 pin 6                         SIN         J2 pin 5
SIN        J1 pin 5                         SOUT        J2 pin 6
SCK        J1 pin 4                         SCK         J2 pin 4
PCS0       J1 pin 3                         PCS0        J2 pin 3
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Short the JP24-2 and JP24-3 with the jumper, other jumpers keep default configuration.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the LOC board J3.
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
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPSPI Master interrupt - FLEXIO SPI Slave interrupt example start.
This example use one lpspi instance as master and one flexio spi slave on one board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is: 
LPSPI_master -- FLEXIO_SPI_slave   
   CLK      --    CLK  
   PCS      --    PCS  
   SOUT     --    SIN  
   SIN      --    SOUT 
This is FLEXIO SPI slave call back.
LPSPI master <-> FLEXIO SPI slave transfer all data matched!

End of Example. 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
