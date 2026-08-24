Hardware requirements
=====================
- USB Type-C cable
- FRDM-MCXA266 board
- HM16 absolute encoder
- BiSS-C electrical interface and encoder power supply
- Personal Computer

Board settings
==============
This example requires connecting the FRDM-MCXA266 board to an HM16 encoder
through an electrical interface that meets the encoder voltage and line-driver
requirements.

- MCXA266 `P2_10` (`FLEXIO0_D18`) -> MA input of the encoder interface
- MCXA266 `P2_9` (`FLEXIO0_D17`) <- SL output of the encoder interface
- FRDM-MCXA266 GND -> encoder interface GND
- Disable the internal pull-up and pull-down on SL

The validated HM16 profile uses the following settings:

- FlexIO root clock: 180 MHz
- BiSS-C bit rate: 10 MHz
- Multi-turn position: 12 bits
- Single-turn position: 16 bits
- ACK: 3 bits
- RX starts on the SL rising edge and samples on the rising edge
- 45 MA clocks per frame
- 39-bit RX capture window with one trailing sample bit removed before parsing
- CRC6 polynomial `0x43`, MSB-first, final XOR `0x3F`

Prepare the Demo
================
1. Connect the HM16 encoder and its electrical interface to the FRDM-MCXA266 board.
2. Connect a USB Type-C cable between the host PC and the board's MCU-Link USB port.
3. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4. Download the program to the target board.
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, information similar to the following is
printed in the terminal window.

~~~~~~~~~~~~~~~~~~~~~
MCUX SDK version: 2026.06.00

FlexIO BiSS-C HM16 demo on MCXA266
FlexIO clock: 180000000 Hz
Bit rate: 10000000 bps, MT: 12 bits, ST: 16 bits, ACK: 3 bits
MA: FLEXIO0_D18, SL: FLEXIO0_D17
RX start: SL rising edge, RX edge: rising, SL pull: off
MA clocks: 45, RX window: 39 bits

MT=2220, ST=63373, ERR=1, WARN=1, CRC=OK
~~~~~~~~~~~~~~~~~~~~~

`ERR` and `WARN` are the raw values received from the encoder. The example does
not infer their active polarity or status meaning.

Use the following combined value to check continuous motion and rollover:

~~~~~~~~~~~~~~~~~~~~~
position = MT * 65536 + ST
~~~~~~~~~~~~~~~~~~~~~

When ST wraps through its 16-bit range and MT increments by one, the combined
position remains continuous.

