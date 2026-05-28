Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMX952 board
- J-Link Debug Probe
- 15V~20V USB Type-C PD power supply
- Personal Computer

Board settings
============
Connect wires on J18 (40-PIN GPIO HDR) to establish connection between FLEXIO1 (master) and LPI2C6 (slave):
- J18-5  (GPIO_IO03, FLEXIO1_FLEXIO[3]) <-> J18-21 (GPIO_IO09, FLEXIO1_FLEXIO[9])  [SCL]
- J18-3  (GPIO_IO02, FLEXIO1_FLEXIO[2]) <-> J18-24 (GPIO_IO08, FLEXIO1_FLEXIO[8])  [SDA]

#### Please note this application can't support running with Linux BSP! ####

Prepare the Demo
===============
1.  Connect 15V~20V USB Type-C PD power supply and J-Link Debug Probe to the board, switch SW1 to power on the board.
2.  Connect a micro USB cable between the host PC and the J1 USB Type-C port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
FlexIO I2C interrupt - LPI2C interrupt
Master will send data :
0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07
0x08  0x09  0x0A  0x0B  0x0C  0x0D  0x0E  0x0F
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1A  0x1B  0x1C  0x1D  0x1E  0x1F

Slave received data :
0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07
0x08  0x09  0x0A  0x0B  0x0C  0x0D  0x0E  0x0F
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1A  0x1B  0x1C  0x1D  0x1E  0x1F
~~~~~~~~~~~~~~~~~~~~~
