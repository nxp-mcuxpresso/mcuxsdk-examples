Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect a Type-C USB cable between the host PC and the MCU-Link port(J16) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================

When the example runs successfully, the following message is displayed in the terminal:

```
 Flash driver API tree demo application.

 Initializing flash driver.
 Flash init successfull!

 Config flash memory access time.

 PFlash Information:
 kFLASH_PropertyPflashBlockBaseAddr = 0x0
 kFLASH_PropertyPflashSectorSize = 8192
 kFLASH_PropertyPflashTotalSize = 262144
 kFLASH_PropertyPflashPageSize = 0x80
 Erase a sector of flash
 Calling flash_verify_erase_sector() API.
 Successfully erased sector: 0x3c000 -> 0x3e000

 Calling FLASH_Program() API.
 Calling FLASH_VerifyProgram() API.
 Successfully programmed and verified location: 0x3c000 -> 0x3c200

 End of PFlash Example!
```

