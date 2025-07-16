Hardware requirements
=====================
- Mini/micro USB cable
- MCXW23-EVK Board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the EVK board J33.
2.  Open a serial terminal with the following settings
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build and download the application
4.  Start a debug session or press the reset button

Running the demo
================
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
2.Going from 12MHz (low freq) to 32MHz (high freq)
3.Going from 32MHz (high freq) to 12MHz (low freq)
Finished FlashUpdateWaitStatesAccordingToMainClk with success !
FLASH_CheckECC found no ECC errors !
