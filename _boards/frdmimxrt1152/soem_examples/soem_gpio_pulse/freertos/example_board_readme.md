Hardware requirements
===================
- Type-C USB cable
- Network cable RJ45 standard
- FRDM-IMXRT1152 board
- BECKHOFF EK1100 EtherCAT Coupler *1
- BECKHOFF EL2008 *1
- BECKHOFF EL1018 *1
- Personal Computer

Board settings
============
- Use the Gigabit Ethernet RJ45 port (J54).
- Set JP2 to position 1-2.

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

