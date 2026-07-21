Hardware requirements
===================
- Type-C USB cable
- Network cable RJ45 standard
- FRDM-IMXRT1186 board
- BECKHOFF EK1100 EtherCAT Coupler *1
- BECKHOFF EL2008 *1
- BECKHOFF EL1018 *1
- Personal Computer

Board settings
============
- J12(1-2)/J13(2-3): ETH0 Function Via J56A.
- J18(1-2)/J17(2-3): ETH2 Function Via J56B.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Power up the EtherCAT Coupler and connect it to the target board(J56-A) via an Ethernet Cable.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Write the program to the flash of the target board.
5.  Press the reset button on your board to start the demo.