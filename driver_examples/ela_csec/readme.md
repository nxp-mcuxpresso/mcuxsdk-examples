# ela_csec

## Overview
This example works only for ELA CSEC enabled parts. SIM_SDID indicates whether ELA CSEC is available on your device.
The first time when running the example on the board, or after a key erase, this example should be ran from RAM.
The user keys are non-volatile. Once the key was loaded, in order to update it, the counter should be increased.
After the user key was loaded using this example, any further full erase of the Flash requires a Challenge-Authentication process.
This can be done by setting the ERASE_ALL_KEYS macro to 1.
After partitioning Flash for ELA CSEC operation, using the JLink Flash configuration of any other project will not work anymore. Workaround:
- Run csec_keyconfig example with ERASE_ALL_KEYS 1, using PEmicro Flash debug configuration

## Running the demo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CSEc Driver Example

CSEc Init - done
Flash is UNSECURE
To access the CSEcfeature set, the part must be configured for EEE operation, using the PGMPART command.
For this purpose FLASH must be parittioned.

Press any key to trigger partitioning...
EEPROM already partitioned, make sure CSEc was enabeled!
CSEc RND Init - done
CSEc Generate random number - success!
CSEc Load plaintext key in KEY_RAM location - success!
CSEc AES-ECB encryption - success!
Encrypted cipher match the expected value!

 End of Example
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-MCXE153](../../_boards/frdmmcxe247/driver_examples/ela_csec/example_board_readme.md)
