Hardware requirements
=====================
- Mini USB cable
- TWR-MC56F8400 board
- Personal Computer

Board hardware common setting
=============================
1. Power setting:
   > Connect J11 pin1 and pin2
   > Connect J6 pin1 and J7 pin2
2. UART setting:
   > Connect J8 pin3 and pin4
   > Connect J9 pin3 and pin4
3. Onboard OSJTAG debugger setting:
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
