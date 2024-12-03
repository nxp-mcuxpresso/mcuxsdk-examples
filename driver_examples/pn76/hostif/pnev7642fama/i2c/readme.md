# hostif_i2c

## Overview
This example shows how to use hostif driver as slave to do board to board transfer 
with interrupt:

In this example, one i2c instance as slave and another i2c instance on the host board as master. 
Master sends a piece of data to slave, and receive a piece of data from slave.