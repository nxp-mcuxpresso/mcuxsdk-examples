# eeprom_emulation

## Overview
The EEPROM Example project is a demonstration program that uses the KSDK software to program eeprom memory
and verify the program.

## Prepare the Demo
Connect a serial cable from the debug UART port of the board to the PC. Start Tera Term
(http://ttssh2.osdn.jp) and make a connection to the virtual serial port.

1. Start Tera Term
2. New connection -> Serial
3. Set apropriate COMx port (x is port number) in Port context menu. Number is provided by operation
   system and could be different from computer to computer. Select COM number related to virtual
   serial port. Confirm selected port by OK button.
4. Set following connection parameters in menu Setup->Serial port...
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: one
5.  Confirm selected parameters by OK button.

## Running the demo
Example output on terminal:

EEPROM example begins...
Page 0 program finished!
Page 1 program finished!
Page 2 program finished!
Page 3 program finished!
Page 4 program finished!
Page 5 program finished!
Page 6 program finished!
Page 7 program finished!
Page 8 program finished!
Page 9 program finished!
...
Page 126 program finished!
All data is correct! EEPROM example succeed!

## Supported Boards
- [FRDM-MCXW71](../../_boards/frdmmcxw71/driver_examples/eeprom_emulation/example_board_readme.md)
- [K32W148-EVK](../../_boards/k32w148evk/driver_examples/eeprom_emulation/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/driver_examples/eeprom_emulation/example_board_readme.md)
