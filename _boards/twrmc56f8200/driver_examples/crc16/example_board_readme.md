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
No special settings are required.

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
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CRC-16 Driver Example.
 test string: 123456789
 CRC-16 CCIT FALSE: 0x29b1
 1.CRC-16/KERMIT : 0x2189
 2.CRC-16/XMODEM : 0x31c3
 3.CRC-16/RIELLO : 0x63d0
 4.CRC-16/TMS37157 : 0x26b1
 5.CRC-16-MCRF4XX : 0x6f91
 6.CRC-16/AUG-CCITT : 0xe5cc
 7.CRC/CRCA : 0xbf05

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

