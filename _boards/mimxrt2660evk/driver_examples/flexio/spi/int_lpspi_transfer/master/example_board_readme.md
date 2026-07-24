Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
To make this example work, please connect FlexIO pins to LPSPI pins:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SLAVE (LPSPI, HSP__LPSPI_1)    MASTER (FlexIO SPI, HSP__FLEXIO_1)
Pin Name   Board Location      Pin Name   Board Location
PCS0       J94-26 (PIO3_13)    PCS        J94-13 (PIO3_0, D00)
SOUT       J94-28 (PIO3_15)    SIN        J94-14 (PIO3_1, D01)
SIN        J94-9  (PIO3_16)    SOUT       J94-15 (PIO3_2, D02)
SCK        J94-27 (PIO3_14)    CLK        J94-16 (PIO3_3, D03)
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

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXIO SPI interrupt example
Master Start...

Master runs successfully!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
