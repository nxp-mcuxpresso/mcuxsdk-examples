Hardware requirements
===================
- Mini/micro USB cable
- Network cable RJ45 standard
- MIMXRT1170-EVKB board
- BECKHOFF EK1100 EtherCAT Coupler *1
- BECKHOFF EL2008 *1
- BECKHOFF EL1018 *1
- Personal Computer

Board settings
============
On MIMXRT1170-EVKB REVC board, GPIO_AD_32 uses as ENET_MDC in this example which is muxed with the SD1_CD_B,
please check the R1926 and R136 connected to SD1_CD_B. If they are populated with resistor and SD card is
inserted, this time enet can't access PHY.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Power up the EtherCAT Coupler and connect it to the target board via an Ethernet Cable.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Write the program to the flash of the target board.
5.  Press the reset button on your board to start the demo.

