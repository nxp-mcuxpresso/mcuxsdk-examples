Hardware requirements
=====================
- Mini USB cable
- TWR-MC56F8200 board
- Personal Computer

Board hardware common setting
=============================
1. HW reworkd(workaround):
   > Connect J18(the mini USB port) pin4 and pin5.
     Note: This is due to HW design error, correct setting should be: J18 pin4 left empty and pin5 connected GND.
2. Power setting:
   > Connect J11 pin1 and pin2
   > Connect J6 pin1 and J7 pin2
3. UART setting: 
   > Connect J8 pin3 and pin4
   > Connect J9 pin3 and pin4
4. Onboard OSJTAG debugger setting:
   > Connect J20 pin1 and pin2.
   > Leave J17 open.
     Note: J17 is only used(connected) when firmware update for onboard debugger OSJTAG.
   > Connect J21 pin 1-2, 3-4, 5-6, 7-8, to setup onboard debugger OSJTAG.
     Note: when use external debugger(connected to J14), leave all J21 pins open

Board settings
==============
- Pull-up SCL/SDA each to 3.3V with resistor(2.2K~5.1K ohm)

  MASTER_BOARD connects to SLAVE_BOARD as below:
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  MASTER_BOARD        CONNECTS TO         SLAVE_BOARD
  Pin Name   Board Location     Pin Name   Board Location
  I2C0_SCL   J501-21            I2C0_SCL   J501-21
  I2C0_SDA   J501-23            I2C0_SDA   J501-23
  GND        J501-8             GND        J501-8
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect USB cable between the host PC and the JM60 USB(J18) port on the target board. It setups OSJTAG and COM port in PC device manager.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with OSJTAG debug configuration.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
I2C board2board polling example -- Master transfer.
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
