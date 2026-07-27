Hardware requirements
=====================
- USB Type-C cable
- FRDM-MCXA366 board
- RS-485 transceiver module (e.g. MAX485 or SN65HVD)
- Tamagawa T-format encoder (e.g. TS5700N8501)
- Personal Computer

Board settings
==============
Connect the RS-485 transceiver to the FRDM-MCXA366 Arduino connector:

| Signal | Arduino Pin | MCU Pin  | FlexIO Channel |
|--------|-------------|----------|----------------|
| DIR    | D16         | PORT2_8  | FLEXIO0_D16    |
| TXD    | D17         | PORT2_9  | FLEXIO0_D17    |
| RXD    | D18         | PORT2_10 | FLEXIO0_D18    |

Prepare the Demo
================
1. Connect a USB Type-C cable between the PC host and the USB port on the FRDM-MCXA366 board.
2. Connect the RS-485 transceiver and T-format encoder as described above.
3. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4. Download the program to the target board.
5. Either re-power up your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
Encoder T-format example
FlexIO Root Clock is 90 MHz
[T-format] Encoder ID: 0x11
        Logic-OR of Over-heat, Multi-turn error, Battery error and Battery alarm
[T-format] Multi-turn data: 1, single-turn data: 66765
        Logic-OR of Over-heat, Multi-turn error, Battery error and Battery alarm
[T-format] Temperature: 34
[1.00s] Encoder ID: 0x11
         Multi-turn data: 1, single-turn data: 66765
         Temperature: 34
[2.00s] Encoder ID: 0x11
         Multi-turn data: 1, single-turn data: 66765
         Temperature: 34
~~~~~~~~~~~~~~~~~~~~~

Note: The "Logic-OR" alarm message is the encoder's own hardware status (battery/overheat
alarm reported via the SF byte) and does not indicate a software error.

