# flexio_endat2_polling_transfer

## Overview
The flexio_endat2_polling_transfer example shows how to use FlexIO as an EnDat2 master in polling mode.

In this example, the FRDM-MCXA366 board connects to an EnDat encoder through an RS-485 interface. FlexIO sends the position command, generates the encoder clock, controls the data direction, receives the position frame, and checks the EnDat CRC5. The decoded multi-turn position, single-turn position, encoder error bit, and CRC result are printed through the debug console.

The default encoder profile is EQI1331_810662-03 with a 12-bit multi-turn field, a 19-bit single-turn field, one error bit, and a 15 MHz EnDat clock.

## Supported Boards
- [FRDM-MCXA366](../../../_boards/frdmmcxa366/digital_encoder_examples/flexio_endat2/polling_transfer/example_board_readme.md)
