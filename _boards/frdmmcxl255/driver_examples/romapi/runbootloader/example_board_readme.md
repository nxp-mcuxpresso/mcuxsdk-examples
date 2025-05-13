Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the LPC-Link USB port (J16) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================

The log below shows example output of the RUNBOOTLOADER driver demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Calling the runBootloader API to force into the ISP mode: 0...
The runBootloader ISP interface is choosen from the following one:
kIspPeripheral_Auto :     0
kIspPeripheral_UsbHid :   1
kIspPeripheral_Uart :     2
kIspPeripheral_SpiSlave : 3
kIspPeripheral_I2cSlave : 4
kIspPeripheral_Can :      5
Call the runBootloader API based on the arg : eb100000...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~