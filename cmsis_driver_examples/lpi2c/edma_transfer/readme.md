# cmsis_lpi2c_edma_transfer

## Overview
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_lpi2c_edma_transfer example shows how to use LPI2C CMSIS driver in EDMA way:

In this example , one lpi2c instance used as LPI2C master and another lpi2c instance used as LPI2C slave .
1. LPI2C master send data to LPI2C slave with EDMA . (LPI2C Slave using interrupt to receive the data)
2. LPI2C master read data from LPI2C slave with EDMA . (LPI2C Slave using interrupt to send the data)
3. LPI2C master abort transfer when send 7 data(LPI2C master send data to LPI2C slave with EDMA.I2C Slave using interrupt to receive the data)
4. LPI2C master read data from LPI2C slave with EDMA in nostop mode. (LPI2C Slave using interrupt to send the data) then LPI2C master read data again. 
5. LPI2C master send data to LPI2C slave with EDMA in nostop mode. (LPI2C Slave using interrupt to read the data) then LPI2C master send data again.

