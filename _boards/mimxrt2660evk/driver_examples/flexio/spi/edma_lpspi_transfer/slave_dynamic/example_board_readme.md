Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
To make the example work, connections needed to be as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER (LPSPI, HSP__LPSPI_1)    SLAVE (FlexIO SPI, HSP__FLEXIO_1)
Pin Name   Board Location       Pin Name   Board Location
PCS0       J94-26 (PIO3_13)     PCS        J94-13 (PIO3_0, D00)
SOUT       J94-28 (PIO3_15)     SIN        J94-14 (PIO3_1, D01)
SIN        J94-9  (PIO3_16)     SOUT       J94-15 (PIO3_2, D02)
SCK        J94-27 (PIO3_14)     CLK        J94-16 (PIO3_3, D03)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
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
