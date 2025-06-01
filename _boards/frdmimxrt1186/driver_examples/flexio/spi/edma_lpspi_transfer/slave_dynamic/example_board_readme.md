Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT1186 board
- Personal Computer

Board settings
==============
Change Jumper R307 from 1-2 to 1-3, Jumper R317 from 1-2 to 1-3.
Change Jumper R112 from 1-2 to 1-3, Jumper R334 from 1-2 to 1-3, Jumper R321 from 1-2 to 1-3, Jumper R138 from 1-2 to 1-3.
To make the example work, connections needed to be as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       MASTER           connect to      SLAVE
Pin Name   Board Location     Pin Name    Board Location
SCK        J2-12                SCK       J61-5
SIN        J2-10                SOUT      J61-7
SOUT       J2-8                 SIN       J61-6
PCS0       J2-6                 PCS0      J61-8
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
You can see the similar message shows following in the terminal if the example runs successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPSPI Master interrupt - FLEXIO SPI Slave edma dynamic transfer example start.
This example use one lpspi instance as master and one flexio spi slave on one board.
Master uses interrupt and slave uses edma way.
Master transfers indefinite amount of data to slave, and the CS signal remains low during transfer.
After master finishes the transfer and re-asserts the CS signal, interrupt is triggered to let user know.
Slave must configure the transfer size larger than master's.
Please make sure you make the correct line connection. Basically, the connection is:
LPSPI_master -- FLEXIO_SPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT
Slave received 128 bytes of data
Slave received 128 bytes of data
LPSPI master <-> FLEXIO SPI slave transfer all data matched!

End of Example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
