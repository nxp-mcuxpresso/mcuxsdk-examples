# hostif_lpuart

## Overview
This example shows how to use hostif driver as slave to do board to board transfer 
with interrupt:

In this example, one lpuart instance as slave and another lpuart instance on the host board as master. 
Master sends a piece of data to slave, and receive a piece of data from slave.