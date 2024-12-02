Overview
========
The lpspi_interrupt_b2b example shows how to use LPSPI driver in interrupt way:

In this example , we need two boards , one board used as LPSPI master and another board used as LPSPI slave.
The file 'dspi_interrupt_b2b_slave.c' includes the LPSPI slave code.
This example does not use the transactional API in LPSPI driver. It's a demonstration that how to use the interrupt in KSDK driver.

1. LPSPI master send/received data to/from LPSPI slave in interrupt . (LPSPI Slave using interrupt to receive/send the data)
Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA276 board
- Personal Computer

Board settings
==============

LPSPI:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       MASTER(SPI1)      connect to        SLAVE(SPI1)
Pin Name   Board Location     Pin Name    Board Location
SOUT       J2-8               SIN         J2-10
SIN        J2-10              SOUT        J2-8
SCK        J2-12              SCK         J2-12
PCS0       J2-6               PCS1        J2-6
GND        J2-14              GND         J2-14
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2. Open a serial terminal on PC for MCU-Link serial device with these settings:
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
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPSPI board to board functional interrupt example.
  Slave start to receive data...

LPSPI transfer all data matched!

 Slave received:

  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
End of slave example!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

