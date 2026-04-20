Hardware requirements
=====================
- Type-C USB cable
- USB-to-serial adapter (e.g. CH340)
- FRDM-MCXL255 board
- Personal Computer

Board settings
============
1. Connect pin 9(RX) and pin 11(TX) of J8 to a USB-to-serial adapter to enable the UART function of the AON domain.
2. The USB-to-serial adapter is used both as the CM0+ debug console and as the wakeup source.

Note: For boards with the schematic SCH-95308 REV E, ensure that SJ20 is connected to pins 2 and 3,
and SJ6 is connected to pins 2 and 3. This configuration allows simultaneous use of LPUART and AON_UART.


Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect a Type-C USB cable between the host PC and the MCU-Link port(J16) on the target board.
2.  Connect a USB-to-serial adapter to J8 pin 9(RX) and pin 11(TX) for the AON LPUART.
3.  Open two serial terminals with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
    One terminal for the CM33 console (MCU-Link CDC), the other for the CM0+ console (USB-to-serial adapter).
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When running the demo, the CM33 debug console shows the menu to command the MCU to the target power mode.
Send any character on the CM0+ (AON LPUART) console to wake the system from the selected low power mode.

CM33 console output:
~~~~~~~~~~~~~~~~~~~~~

###########################  AON LPUART Wakeup Demo (CM33)  ###########################
Normal Boot......
Core Clock Frequency: 96000000

Select power mode (AON LPUART wakeup)...
	Press A: Active --> Sleep --> Active
	Press B: Active --> Deep Sleep --> Active
	Press C: Active --> Power Down1 --> Active
	Press D: Active --> Power Down2 --> Active
	Press E: Active --> Deep Power Down1 --> Active
	Press F: Active --> Deep Power Down2 --> Active

Waiting for selection...

~~~~~~~~~~~~~~~~~~~~~

CM0+ console output:
~~~~~~~~~~~~~~~~~~~~~

###########################  AON LPUART Wakeup Demo (CM0+)  ###########################
Core Clock Frequency: 10000000
Syncing with CM33
System Is In Active Mode!

~~~~~~~~~~~~~~~~~~~~~
