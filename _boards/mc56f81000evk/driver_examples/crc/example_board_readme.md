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
No special settings are required.

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
The log below shows example output of the CRC driver demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CRC Peripheral Driver Example.

Test same string base on different CRC protocols.
 test string: 123456789
 1.CRC-16 CCIT FALSE: 0x29b1
 2.CRC-16 KERMIT: 0x2189
 3.CRC-16 MAXIM: 0x44c2
 4.CRC-32: 0xcbf43926
 5.CRC-32 POSIX: 0x765e7680

Test different strings based on CRC-32 POSIX protocol.
 1.test string: abcdefgh
  CRC-32 POSIX: 0x5024ec61
 2.test string: abcd1234
  CRC-32 POSIX: 0x3fbda0ab

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
