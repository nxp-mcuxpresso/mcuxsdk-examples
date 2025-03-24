# mcxw23_romapi_flash

## Overview

The IAP Flash project is a simple demonstration program of the SDK IAP Flash driver. 
A message is printed on the UART terminal when various operations are performed on Flash memory.

## Running the demo

The log below shows example output of the IAP Flash demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        ----- Flash driver IAP Demo Application -----
Initializing Flash driver...
Flash Init done with success !
        PflashTotalSize = 1040384
        PflashSectorSize = 8192
        PflashPageSize = 128
        PflashBlockSize = 1040384
        PflashBlockCount = 1
        PflashBlockBaseAddr = 0
        PflashSystemFreqMHz = 32
Flash Properties printed with success !
Working on Flash Address : 0xFC000
Finished 'Erase/Write' Scenario with success !
Finished 'Read/Modify/Write' Scenario with success !
Executing interrupts from RAM during flash operations: USER LED should start blinking ..
Finished FlashEraseProgramDuringRAMInterrupt with success !
1.Going from 32MHz (high freq) to 12MHz (low freq)
2.Going from 12MHz (low freq)  to 32MHz (high freq)
3.Going from 32MHz (high freq) to 12MHz (low freq)
Finished FlashUpdateWaitStatesAccordingToMainClk with success !

## Supported Boards
- [FRDM-MCXW23](../../../_boards/frdmmcxw23/driver_examples/romapi/flashiap/example_board_readme.md)
- [MCXW23-EVK](../../../_boards/mcxw23evk/driver_examples/romapi/flashiap/example_board_readme.md)
