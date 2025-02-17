# bee

## Overview
This driver example should demonstrate how to setup NPX driver for on the fly encryption/decryption of data stored in flash memory region.
The NPX is configured for the on fly encryption/decryption all data starting from location 0x001A0000 to 0x001AFFFF.
The example enable NPX, writen data is on the fly encrypted into specified region, when NPX is enabled read from that region is same as data writen as NPX performs on the fly decryption.
Then NPX is disabled and readed data doesnt match expected value as on the fly decryption didnt happen

## Supported Boards
- [FRDM-MCXN236](../../_boards/frdmmcxn236/driver_examples/npx/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/driver_examples/npx/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/driver_examples/npx/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/driver_examples/npx/example_board_readme.md)
