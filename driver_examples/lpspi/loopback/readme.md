# lpspi_loopback

## Overview

The lpspi_loopback demo shows how the lpspi do a loopback transfer, LPSPI
master will transmit data to itself, so please connect the SOUT pin to SIN
pin directly.

## Running the demo

When the example runs successfully, you can see the similar information from the terminal as below.

LPSPI functional interrupt example start.
This example use one lpspi instance as master and another as slave on one board.
Master uses interrupt way and slave uses interrupt way.
Note that some LPSPI instances interrupt is in INTMUX ,you should set the intmux when you porting this example accordingly
Please make sure you make the correct line connection. Basically, the connection is:
LPSPI
   SOUT     --    SIN

LPSPI loopback example start
LPSPI loopback test pass!!!

## Supported Boards
