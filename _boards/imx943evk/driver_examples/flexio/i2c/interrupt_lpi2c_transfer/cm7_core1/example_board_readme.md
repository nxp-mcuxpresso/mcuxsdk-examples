Hardware requirements
=====================
- Micro USB cable
- IMX943-EVK  board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
============
The example requires doing connection between FLEXIO1(as master) pins and LPI2C4(as slave) pins.
- LPI2C4_SCL(J12-5), HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO31__FLEXIO1_3_1_FLEXIO5(J44-10) connected
- LPI2C4_SDA(J12-3), HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO30__FLEXIO1_3_1_FLEXIO4(J51-18) connected

#### Please note this application can't support running with Linux BSP! ####

Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW301 to power on the board.
2.  Connect a micro USB cable between the host PC and the J1401 USB port on the target board.
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
