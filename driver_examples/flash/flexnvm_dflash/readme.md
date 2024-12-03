# flexnvm_dflash

## Overview
The flexnvm_dflash example shows how to use flash driver to operate data flash:


## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.

1. For parts with FlexNVM:
~~~~~~~~~~~~
 FlexNVM DFlash example Start
 Flash is UNSECURE!
 DFlash Information: 
 Data Flash Base Address: (xx)
 Data Flash Total Size: xx KB, hex: (xx)
 Data Flash Sector Size: xx KB, hex: (xx)
 Erase a sector of Data Flash
 Successfully Erased Sector xx -> xx
 Program a buffer to a sector of Data Flash
 Successfully Programmed and Verified Location xx -> xx


 End of FlexNVM Dflash example
~~~~~~~~~~~~

2. For Some parts Without FlexNVM (such as K65FN):
~~~~~~~~~~~~
 FlexNVM DFlash example Start
 Current device doesn't support FlexNVM feature


 End of FlexNVM Dflash example
~~~~~~~~~~~~

## Supported Boards
- [FRDM-KE15Z](../../../_boards/frdmke15z/driver_examples/flash/flexnvm_dflash/example_board_readme.md)
