Hardware requirements
=====================
- 2 x Micro USB cable
- MC56F81000-EVK board
- Personal Computer

Board hardware common setting
=============================
1. Power setting:
   > Connect J8 pin2 and pin3
   > Connect J5 pin1 and pin2
   > Connect J6 pin1 and pin2
2. Onboard PnE U-MultiLink debugger setting:
   > Connet J11 pin2 and pin3
   > Connect J13 pin 1-2, 3-4, 5-6, 7-8, to setup onboard debugger PnE U-MultiLink.
     Note: when use external debugger(connected to J10), leave all J13 pins open.

Board settings
==============
  Transfer data from LPI2C0 as master to LPI2C1 as slave
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  MASTER_BOARD        CONNECTS TO         SLAVE_BOARD
  Pin Name   Board Location     Pin Name   Board Location
  I2C0_SCL   J2-20              I2C1_SCL   J3-5
  I2C0_SDA   J2-18              I2C1_SDA   J3-7
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect USB cable between the host PC and the USB(J26) port on the target board to setup USB to UART bridge.
2.  Connect USB cable between the host PC and the USB(J12) port on the target baord to setup mulitlink debugger.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board with PnE U-MultiLink debug configuration.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
    Please be noted default FCF(flash configuration field) makes the MCU boot from flash.

Running the demo
================

When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
LPI2C example -- MasterFunctionalInterrupt_SlaveFunctionalInterrupt.
Master will send data :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f

Slave received data :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f

This time , slave will send data:
0xff  0xfe  0xfd  0xfc  0xfb  0xfa  0xf9  0xf8
0xf7  0xf6  0xf5  0xf4  0xf3  0xf2  0xf1  0xf0
0xef  0xee  0xed  0xec  0xeb  0xea  0xe9  0xe8
0xe7  0xe6  0xe5  0xe4  0xe3  0xe2  0xe1  0xe0

Master received data :
0xff  0xfe  0xfd  0xfc  0xfb  0xfa  0xf9  0xf8
0xf7  0xf6  0xf5  0xf4  0xf3  0xf2  0xf1  0xf0
0xef  0xee  0xed  0xec  0xeb  0xea  0xe9  0xe8
0xe7  0xe6  0xe5  0xe4  0xe3  0xe2  0xe1  0xe0

End of I2C example .
~~~~~~~~~~~~~~~~~~~~~

