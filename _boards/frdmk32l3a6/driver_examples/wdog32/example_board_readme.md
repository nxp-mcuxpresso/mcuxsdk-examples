Hardware requirements
=====================
- Mini USB cable
- FRDM-K32L3A6 board
- Personal Computer

Board settings
==============
This WDOG32 Example project does not call for any special hardware configurations. Although not required, the recommendation is to leave the development
board's jumper settings and configurations in default state when running this example.
Note: By default, the MCU will boot from ROM, this will affect wdog, so make sure MCU boots from flash before running this example.
      After downloading the codes, please reset the MCU, in debug mode, wdog maybe not work correctly.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, will get the similar messages on the OpenSDA terminal.

~~~~~~~~~~~~~~~~~~~~~~
Low Byte test success
High Byte test success

----- Refresh test start -----
----- None-window mode -----
Refresh wdog32 0 time
Refresh wdog32 1 time
Refresh wdog32 2 time
Refresh wdog32 3 time
Refresh wdog32 4 time
Refresh wdog32 5 time
Refresh wdog32 6 time
Refresh wdog32 7 time
Refresh wdog32 8 time
Refresh wdog32 9 time

----- Window mode -----
Refresh wdog32 0 time
Refresh wdog32 1 time
Refresh wdog32 2 time
----- Refresh test success  -----

----- End of WDOG32 example  -----
~~~~~~~~~~~~~~~~~~~~~~