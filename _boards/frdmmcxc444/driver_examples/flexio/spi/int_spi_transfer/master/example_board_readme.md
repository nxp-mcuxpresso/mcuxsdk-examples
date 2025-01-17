Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXC444 board
- Personal Computer

Board settings
============
The flexio_spi_master_interrupt_spi_slave example is requires connecting between FlexIO pins with SPI pins
The connection should be set as following:
	FLEXIO       SPI1
PCS0    J4-1         J2-6
SCK     J4-3         J2-12
MISO    J2-10        J2-18
MOSI    J2-8         J2-20

Prepare the Demo
===============
1.  Connect a type-c USB cable between the host PC and the MCU-Link USB port (J13) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~
FLEXIO Master - SPI Slave interrupt example start.

This example use one flexio spi as master and one spi instance as slave on one board.

Master and slave are both use interrupt way.

Please make sure you make the correct line connection. Basically, the connection is:

FLEXI_SPI_master -- SPI_slave

   SCK      --    SCK

   PCS0     --    PCS0

   MOSI     --    MOSI

   MISO     --    MISO

This is SPI slave call back.

FLEXIO SPI master <-> SPI slave transfer all data matched!
~~~~~~~~~~~~~~~~~~~~~
