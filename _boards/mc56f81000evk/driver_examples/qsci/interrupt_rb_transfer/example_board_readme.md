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
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
QSCI RX ring buffer example
Send back received data
Echo every 8 bytes
Do not input more than 20 bytes of characters at one time!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
