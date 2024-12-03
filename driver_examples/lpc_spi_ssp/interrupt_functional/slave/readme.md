# spi_interrupt_functional_slave

## Overview
The spi_interrupt_functional_slave example shows how to use spi driver as slave to receive data from master.

In this example, one spi instance as slave and another spi instance on other board as master. 
Master sends a piece of data to slave, and receive a piece of data from slave. This example 
checks if the data received from master is correct. This example should work with 
spi_interrupt_master example. And this example should start first.
