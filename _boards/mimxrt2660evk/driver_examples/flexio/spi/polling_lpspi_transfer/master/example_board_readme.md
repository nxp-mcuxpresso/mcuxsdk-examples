Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
FLEXIO SPI master (HSP__FLEXIO_0) is used to connect with LPSPI slave (HSP__LPSPI_1).
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SLAVE (LPSPI, HSP__LPSPI_1)      MASTER (FlexIO SPI, HSP__FLEXIO_1)
Pin Name   Board Location        Pin Name   Board Location
PCS0       J94-26 (PIO3_13)      PCS        J94-13 (PIO3_0, D00)
SOUT       J94-28 (PIO3_15)      SIN        J94-14 (PIO3_1, D01)
SIN        J94-9  (PIO3_16)      SOUT       J94-15 (PIO3_2, D02)
SCK        J94-27 (PIO3_14)      CLK        J94-16 (PIO3_3, D03)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the Demo
================
When the example runs successfully, you can see the similar information from the terminal as shown below.

```
FLEXIO Master - LPSPI Slave polling example start.
This example use one flexio spi as master and one lpspi instance as slave on one board.
Master and slave are both use polling way.
Please make sure you make the correct line connection. Basically, the connection is:
FLEXIO_SPI_master -- LPSPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SDI
   SIN      --    SDO

Master transmit:
 1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40

Master received:
 1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40

FLEXIO SPI master <-> LPSPI slave transfer all data matched!

End of master example!
```
