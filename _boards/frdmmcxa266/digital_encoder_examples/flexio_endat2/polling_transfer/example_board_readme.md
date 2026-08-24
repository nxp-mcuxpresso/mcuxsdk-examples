Hardware requirements
=====================
- USB Type-C cable
- FRDM-MCXA266 board
- EQI1331_810662-03 absolute encoder
- EnDat2 RS-485 electrical interface and encoder power supply
- Personal Computer

Board settings
==============
This example requires connecting the FRDM-MCXA266 board to an EnDat2 encoder
through an RS-485 electrical interface that meets the encoder voltage and
line-driver requirements.

- MCXA266 `P2_8` (`FLEXIO0_D16`) -> TXD input of the RS-485 interface
- MCXA266 `P2_9` (`FLEXIO0_D17`) <- RXD output of the RS-485 interface
- MCXA266 `P2_10` (`FLEXIO0_D18`) -> CLK input of the RS-485 interface
- MCXA266 `P2_11` (`FLEXIO0_D19`) -> DIR input of the RS-485 interface
- FRDM-MCXA266 GND -> encoder interface GND
- Remove the external pull-up on RXD and enable the internal pull-down on
  `P2_9` (`FLEXIO0_D17`)

The internal pull-down on RXD is required because the RS-485 receiver's RO
output is high-impedance while idle, so RXD must read low for the example to
correctly detect the first RXD rising edge after DIR goes low as the EnDat2
Start bit.

The validated encoder profile uses the following settings:

- FlexIO root clock: 60 MHz (PLL1_DIV)
- EnDat2 bit rate: 15 MHz
- Multi-turn position: 12 bits
- Single-turn position: 19 bits
- ERR1: 1 bit
- CRC5 polynomial `x^5 + x^3 + x^1 + 1`

Prepare the Demo
================
1. Connect the EQI1331_810662-03 encoder and its RS-485 electrical interface to the FRDM-MCXA266 board.
2. Power the encoder and confirm the interface logic voltage is compatible with the MCXA266 I/O voltage.
3. Connect a USB Type-C cable between the host PC and the board's MCU-Link USB port.
4. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
5. Download the program to the target board.
6. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, information similar to the following is
printed in the terminal window.

~~~~~~~~~~~~~~~~~~~~~
MCXA266 FlexIO EnDat2 demo
Encoder: EQI1331_810662-03, MT=12, ST=19, bitrate=15000000 bps
FlexIO clock: 60000000 Hz
Pins: TXD=FLEXIO0_D16, RXD=FLEXIO0_D17, CLK=FLEXIO0_D18, DIR=FLEXIO0_D19

MT=2750, ST=519896, ERR1=0, CRC=OK
MT=2751, ST=201629, ERR1=0, CRC=OK
~~~~~~~~~~~~~~~~~~~~~

`ERR1` is the raw error bit received from the encoder. `CRC=OK` means the
received frame passed CRC5 validation.

If `No EnDat2 start bit detected` is printed, check the RXD idle level, the
DIR polarity, the RS-485 receiver enable wiring, the encoder power, and the
differential DATA wiring. If `CRC=FAIL` is printed repeatedly, check the
configured bit rate and field lengths, signal integrity, clock/data polarity,
and termination.
