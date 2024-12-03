# freertos_ecspi_loopback

## Overview
The freertos_ecspi_loopback demo shows how the ecspi do a loopback transfer internally in FreeRTOS.
The ECSPI connects the transmitter and receiver sections internally, and the data shifted out from the 
most-significant bit of the shift register is looped back into the least-significant bit of the Shift register. 
In this way, a self-test of the complete transmit/receive path can be made. The output pins are not affected, 
and the input pins are ignored.

## Supported Boards
