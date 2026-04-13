Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer
- Serial port adapter (for AON UART)

Board settings
============
1. Connect pin 9(RX) and pin 11(TX) of J8 to a serial port adapter to enable the UART function of the AON domain.

Note: For boards with the schematic SCH-95308 REV E, ensure that SJ20 is connected to pins 2 and 3,
and SJ6 is connected to pins 2 and 3. This configuration allows simultaneous use of LPUART and AON_UART.

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect a Type-C USB cable between the host PC and the MCU-Link port(J16) on the target board.
2.  Open a serial terminal on the AON UART (via J8 serial adapter) with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When running the demo, the AON UART shows CM33 boot messages, then CM0+ heartbeat logs during DPD1.
Send any character on the AON UART to trigger CM33 wakeup with the CGU workaround.

~~~~~~~~~~~~~~~~~~~~~

###########################  DPD1 AON Monitor Demo — CM33 Boot  ###########################
Core Clock: 96000000 Hz
Copy CM0+ image to 0xa1000000, size: XXXX
Syncing with CM0+...
Dual-core sync OK
Press any key to start the demo...

Entering DPD1 (context saving) — CM0+ stays active...
Send any character on AON UART to wake up.

### DPD1 AON Monitor — CM0+ Boot ###
CM0+ active in DPD1 — monitoring AON UART RX
Send any character to wake CM33...

[DPD1] Heartbeat #0  (CM0+ alive, CM33 Powered Off)
[DPD1] Heartbeat #1  (CM0+ alive, CM33 Powered Off)
[DPD1] Heartbeat #2  (CM0+ alive, CM33 Powered Off)

[DPD1] UART RX detected — waking CM33 (with CGU workaround)
Triggering CM33 wakeup...
CGU restored — CM33 is ready
[DPD1] CM33 wakeup complete — returning to Active mode

*** Wakeup from DPD1 ***

~~~~~~~~~~~~~~~~~~~~~
