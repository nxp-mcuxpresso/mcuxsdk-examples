Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXE32B board
- Personal Computer

Board settings
==============
The example requires connecting the FLEXIO pins with the LPSPI pins
The connection should be set as following:

MASTER(FlexIO SPI)    connect to    SLAVE(LPSPI)
Pin Name   Board Location           Pin Name  Board Location
SOUT       J9-8                     SIN       J2-10
SIN        J9-7                     SOUT      J2-8
SCK        J9-10                    SCK       J2-12
PCS        J9-9                     PCS2      J2-6

Prepare the Demo
================
1. Connect a USB cable between the PC host and the MCU-Link USB port on the board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
FLEXIO Master edma - LPSPI Slave interrupt example start.

This example use one flexio spi as master and one lpspi instance as slave on one board.

Master uses edma and slave uses interrupt way.

Please make sure you make the correct line connection. Basically, the connection is:

FLEXI_SPI_master -- LPSPI_slave

      CLK        --    CLK

      PCS        --    PCS

      SOUT       --    SIN

      SIN        --    SOUT

This is LPSPI slave call back.

FLEXIO SPI master <-> LPSPI slave transfer all data matched!

End of example.
~~~~~~~~~~~~~~~~~~~~~
