# hostif_spi

## Overview
This example shows how to use hostif driver as slave to do board to board transfer 
with interrupt:

In this example, one spi instance as slave and another spi instance on the host board as master. 
Master sends a piece of data to slave, and receive a piece of data from slave.