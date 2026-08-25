Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA577 board
- Personal Computer
- Loopback network cable RJ45 standard

Board settings
============
Short position 2-3 on J29.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert loopback network cable to Ethernet RJ45 port.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

The example uses external PHY over MII by default (ERR053383: A0 silicon has an ENET RMII RX issue). If you want to use the internal 10BASE-T1S digital PHY,
redefine BOARD_NETWORK_USE_TENBASET_PHY from board.h to 1 and rebuild. Loopback cable is not needed in that case.

Note: In the default MII configuration the ENET TX bus uses P1_8/P1_9, which are the same pins as the MCU-Link VCOM debug console (LPUART1).
Therefore this example routes its debug console to LPUART2 on the Arduino header (D1/TX = P2_10, D0/RX = P2_11) instead of the VCOM port.
Connect an external USB-to-UART module to those Arduino-header pins (module RX to D1/P2_10, module TX to D0/P2_11, GND to GND) and open the serial
terminal on that module's COM port. When using the internal 10BASE-T1S PHY (BOARD_NETWORK_USE_TENBASET_PHY = 1), the console stays on the MCU-Link VCOM port.


Make loopback network cable:

| Pin | 568B standard | Unknown standard |
|-----|---------------|------------------|
| J1  | orange+white  | green+white      |
| J2  | orange        | green            |
| J3  | green+white   | orange+white     |
| J4  | blue          | brown+white      |
| J5  | blue+white    | brown            |
| J6  | green         | orange           |
| J7  | brown+white   | blue             |
| J8  | brown         | blue+white       |

Connect J1 => J3, J2 => J6, J4 => J7, J5 => J8. 10/100M transfer only requires J1, J2, J3, J6, and 1G transfer requires all 8 pins.
Check your net cable color order and refer to 568B standard or the other standard. If your cable's color order is not shown in the list,
please connect J1~J8 based on your situation.

Running the demo
================
The log below shows example output of the example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 ENET example start.
Wait for PHY init...
Wait for PHY link up...

Transmission start now!
The 1 frame transmitted success!
1 frame has been successfully received
The 2 frame transmitted success!
2 frame has been successfully received
The 3 frame transmitted success!
3 frame has been successfully received
The 4 frame transmitted success!
4 frame has been successfully received
The 5 frame transmitted success!
5 frame has been successfully received
The 6 frame transmitted success!
6 frame has been successfully received
The 7 frame transmitted success!
7 frame has been successfully received
The 8 frame transmitted success!
8 frame has been successfully received
The 9 frame transmitted success!
9 frame has been successfully received
The 10 frame transmitted success!
10 frame has been successfully received
The 11 frame transmitted success!
11 frame has been successfully received
The 12 frame transmitted success!
12 frame has been successfully received
The 13 frame transmitted success!
13 frame has been successfully received
The 14 frame transmitted success!
14 frame has been successfully received
The 15 frame transmitted success!
15 frame has been successfully received
The 16 frame transmitted success!
16 frame has been successfully received
The 17 frame transmitted success!
17 frame has been successfully received
The 18 frame transmitted success!
18 frame has been successfully received
The 19 frame transmitted success!
19 frame has been successfully received
The 20 frame transmitted success!
20 frame has been successfully received

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Optionally, you may use a loop back cable
====================================
Make a loopback network cable:
      568B standard 	 Unknowed standard
J1    orange+white       green+white
J2    orange             green
J3    green+white        orange+white
J4    blue               brown+white
J5    blue+white         brown
J6    green              orange
J7    brown+white        blue
J8    brown              blue+white

Connect J1 => J3, J2 => J6, J4 => J7, J5 => J8. 10/100M transfer only requires J1, J2, J3, J6, and 1G transfer requires all 8 pins.
Check your net cable color order and refer to the 568B standard or the other standard. If your cable's color order is not showed in the list,
please connect J1~J8 based on your situation.

1.  Set CONFIG_APP_USES_LOOPBACK_CABLE=y in .config (or add/change #define APP_USES_LOOPBACK_CABLE 1 in mcux_config.h if you don't use Kconfig) and rebuild the example.
2.  Insert loopback network cable to Ethernet RJ45 port.
3.  Run the demo in the same way as described earlier.