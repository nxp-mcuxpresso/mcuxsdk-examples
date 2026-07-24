Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
- Connect the SPI signals of the LPSPI master (acting as SPI master) with the SPI signals of the FlexIO SPI slave (acting as SPI slave).
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
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Connect the pins as described in "Board settings" section.
3. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4. Download the program to the target board.
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
LPSPI Master interrupt - FlexIO SPI Slave interrupt example start.

This example uses one lpspi instance as master and one flexio spi slave on one board.

Master and slave are both use interrupt way.

Please make sure you make the correct line connection. Basically, the connection is:

LPSPI_master -- FlexIO_SPI_slave

   CLK      --    CLK

   PCS      --    PCS

   SOUT     --    SIN

   SIN      --    SOUT

This is LPSPI master call back.

LPSPI master <-> FLEXIO SPI slave transfer all data matched!

End of Example.
~~~~~~~~~~~~~~~~~~~~~
