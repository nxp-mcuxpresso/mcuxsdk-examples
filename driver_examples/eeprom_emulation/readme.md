# eeprom_emulation

## Overview
The EEPROM Emulation example shows how to emulate EEPROM on a flash type memory.

## Prepare the Demo
1. Connect the micro and mini USB cable between the PC host and the USB ports on the board.
2. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

## Running the demo
Upon successfull initialization of the example you should see the following lines in the terminal:

    -- EEPROM EMULATION EXAMPLE --

    Using the following setup:
      EEPROM size:  64
      FLASH offset: 0xc000
      FLASH sector size:    8192
      FLASH sectors used:   2
      Change Block size:    16
      RAM buffer used:      YES

    Erasing flash area used for EEPROM emulation...
    EEPROM readback of the erased FLASH area:
     40000fc  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|
     400010c  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|
     400011c  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|
     400012c  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|

    Initializing EEPROM content with the first write...

    EEPROM readback of the initialized FLASH area:
     40000fc   0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 |................|
     400010c   0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 |................|
     400011c   0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 |................|
     400012c   0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 |................|

    EEPROM write of length 27...
    EEPROM readback and verify:
     40000fc  53 4F 4D 45 20 52 41 4E 44 4F 4D 20 45 45 50 52 |SOME RANDOM EEPR|
     400010c  4F 4D 20 43 4F 4E 54 45 4E 54  0  0  0  0  0  0 |OM CONTENT......|
     400011c   0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 |................|
     400012c   0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 |................|

    Verified OK

    Now going to test EEPROM rollover to next FLASH sector

    ...

## Supported Boards
- [FRDM-MCXC162](../../_boards/frdmmcxc162/driver_examples/eeprom_emulation/example_board_readme.md)
