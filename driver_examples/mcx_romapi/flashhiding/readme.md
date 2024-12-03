# mcxromapi_flash_hiding

## Hardware requirements
- Mini/micro USB cable
- LPCXpresso55S36 board
- Personal Computer

## Board settings

## Prepare the Demo
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J1) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

## Running the demo

When the example runs successfully, the following message is displayed in the terminal:

```
Flash Hiding example...

FLASH_Init PASSED
FFR_Init PASSED
Erasing FLASH sectors, address: 10000, size:8000
flash_page_erase PASSED
Programming FLASH, address: 10000, size: 1000
flash_page_program PASSED
FLASH hiding no enabled and read PASSED!
MBCAddr = 0
MBCAddr = 500
FLASH hiding enabled and read limit PASSED
flash hiding test erase and program PASSED


CDPA example...

FLASH_Init PASSED
FFR_Init PASSED
Before enabling the CDPA, call flash_erase API and flash_program API ...
Erasing FLASH sectors, address: 18000, size:8000
flash_page_erase PASSED
Programming FLASH, address: 18000, size: 1000
CDPA no enabled and flash_program PASSED
CDPABlock = 0
CDPABlock = 4000
CDPA enabled and read test PASSED
CDPA test erase and program PASSED

```

## Supported Boards
- [FRDM-MCXN236](../../../_boards/frdmmcxn236/driver_examples/romapi/flashhiding/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/romapi/flashhiding/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/romapi/flashhiding/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/romapi/flashhiding/example_board_readme.md)
