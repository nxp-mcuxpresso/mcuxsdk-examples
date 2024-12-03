Hardware requirements
=====================
- Micro USB cable
- MC56F83000-EVK board
- Personal Computer

Board hardware common setting
=============================
1. Power setting:
   > Connect J19 pin2 and pin3
   > Connect J12 pin1 and pin2
   > Connect J13 pin1 and pin2
   > Connect J15 pin1 and pin2
   > Connect J16 pin2 and pin3
   > Connect J17 pin1 and pin2
2. Onboard OSJTAG debugger setting:
   > Leave J5 open
   > Leave J6 open
     Note: J6 is only used(connected) when firmware update for onboard debugger OSJTAG.
   > Connect J11 pin 1-2, 3-4, 5-6, 7-8, to setup onboard debugger OSJTAG.
     Note: when use external debugger(connected to J10), leave all J11 pins open.

Board settings
==============
  MASTER_BOARD connects to SLAVE_BOARD as below:
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  MASTER_BOARD        CONNECTS TO         SLAVE_BOARD
  Pin Name   Board Location     Pin Name   Board Location
  I2C0_SCL   J2-20              I2C0_SCL   J2-20
  I2C0_SDA   J2-18              I2C0_SDA   J2-18
  GND        J2-14              GND        J2-14
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect USB cable between the host PC and the JM60 USB(J8) port on the target board. It setups OSJTAG and COM port in PC device manager.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with OSJTAG debug configuration.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo. 
    Please be noted that default FCF(flash configuration field) setting makes MCU boot from bootloader. So after reset button press,
    it will wait 5s(wait in bootloader) to run the application code.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
I2C board2board interrupt example -- Master transfer.
Master will send data :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7  
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f  
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17  
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f  

Receive sent data from slave :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7  
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f  
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17  
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f  


End of I2C example .
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
