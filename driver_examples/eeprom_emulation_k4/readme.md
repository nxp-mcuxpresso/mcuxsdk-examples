# eeprom_emulation_k4

## Overview
The EEPROM Example project is a demonstration program that uses the KSDK software to program eeprom memory
and verify the program.

##Prepare the Demo
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

##Running the demo
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
EEPROM demo begins...
All data is correct! EEPROM Emulation example succeed!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-MCXW71](../../_boards/frdmmcxw71/driver_examples/eeprom_emulation/example_board_readme.md)
- [MCX-W71-EVK](../../_boards/mcxw71evk/driver_examples/eeprom_emulation/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/driver_examples/eeprom_emulation/example_board_readme.md)
