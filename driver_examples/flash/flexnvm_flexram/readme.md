# flexnvm_flexram

## Overview
The flexnvm_flexram example shows how to use flash driver to operate flexram as traditional RAM:


## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.

1. For parts with FlexNVM:
~~~~~~~~~~~~
 FlexNVM FlexRAM Example Start
 Flash is UNSECURE!
 FlexRAM Information: 
 FlexRAM Base Address: (0xxx)
 FlexRAM Total Size: xx B
 Make FlexRAM available for traditional RAM
 FlexNVM FlexRAM Example Start
 Flash is UNSECURE!
 FlexRAM Information: 
 FlexRAM Base Address: (0xxx)
 FlexRAM Total Size: xx B
 Program the first 16 bytes memory space of FlexRAM as 10,11,12,13.
 Read 16 bytes data from start of FlexRAM space
 The first 16 bytes data in EEPROM are what we have progrommed before
 Read the first 16 bytes memory space of FlexRAM as10,11,12,13.
 Successfully Programmed Location xx -> xx

 End of FlexNVM EEprom example
~~~~~~~~~~~~

2. For Some parts Without FlexNVM (such as K65FN):
~~~~~~~~~~~~
 FlexNVM FlexRAM example Start
 Current device doesn't support FlexNVM feature


 End of FlexNVM EEprom example
~~~~~~~~~~~~

## Supported Boards
- [FRDM-KE15Z](../../../_boards/frdmke15z/driver_examples/flash/flexnvm_flexram/example_board_readme.md)
