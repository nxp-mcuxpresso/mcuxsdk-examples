Hardware requirements
=====================
- Type-C USB cable
- IMX95LP4X-EVK  board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
============
Set SW2[1] on
Set SW10[1] on
connect J27-3 to J27-24
connect J27-5 to J27-21

- HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO09__FLEXIO1_FLEXIO_BIT9 , HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO03__LPI2C6_SCL connected
- HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO08__FLEXIO1_FLEXIO_BIT8 , HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO02__LPI2C6_SDA connected

**Please note this application can't support running with Linux BSP!**

Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW4 to power on the board.
2.  Connect a micro USB cable between the host PC and the J31 USB port on the target board.
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
