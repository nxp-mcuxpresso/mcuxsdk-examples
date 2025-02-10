# csec

## Overview
This example works only for CSEc enabled parts. SIM_SDID indicates whether CSEc is available on your device.
The first time when running the example on the board, or after a key erase, this example should be ran from RAM.
The user keys are non-volatile. Once the key was loaded, in order to update it, the counter should be increased.
After the user key was loaded using this example, any further full erase of the Flash requires a Challenge-Authentication process.
This can be done by setting the ERASE_ALL_KEYS macro to 1.
After partitioning Flash for CSEc operation, using the JLink Flash configuration of any other project will not work anymore. Workaround:
- Run csec_keyconfig example with ERASE_ALL_KEYS 1, using PEmicro Flash debug configuration

## Supported Boards
- [FRDM-MCXE153](../../_boards/frdmmcxe247/driver_examples/csec/example_board_readme.md)
