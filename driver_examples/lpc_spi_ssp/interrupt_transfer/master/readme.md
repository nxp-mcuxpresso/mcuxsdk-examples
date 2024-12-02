# spi_interrupt_transfer_master

## Overview
The spi_interrupt_transfer_master example shows how to use spi driver as master to do board to board
transfer in interrupt way:

In this example, one spi instance as master and another spi instance on other board as slave. Master
sends a piece of data to slave, and receive a piece of data from slave. This example checks if the 
data received from slave is correct. This example need to work with spi_interrupt_transfer_slave example.
  