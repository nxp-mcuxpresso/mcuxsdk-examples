# spi_half_duplex_polling_master

## Overview
The lpc_spi_half_duplex_polling_transfer_master example shows how to use driver API to transfer in half-duplex way.

In this example, one spi instance as master and another spi instance on the othere board as slave. Master sends 
a piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.
Besides, master will transfer data in polling way. 
