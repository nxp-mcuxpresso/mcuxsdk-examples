Hardware requirements
=====================
- Mini/micro USB cable
- LPCLPCXpresso54S018 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
This demo is an external flash plain load demo, ROM will copy image in external flash to SRAMX to run:
1. Build the demo to generate a bin file.
   Note: If need to generate bin file using MCUXpresso IDE, below steps need to be followed:
         Set in example Properties->C/C++ Build->Settings->Build steps->Post-build steps->Edit
         enbable arm-none-eabi-objcopy -v -O binary "&{BuildArtifactFileName}" "&{BuildArtifactFileBaseName}.bin" 
         
         This plainload example linked the vector table to 0x00000000, but program to external flash 0x10000000.

2. Program the bin file to external on board flash via SEGGER J-FLASH Lite(V6.22 or higher):

   a. Open SEGGER J-FLASH Lite, select device LPC54S018.

   b. Click the 'Erase Chip' to erase the extrenal flash.(if can not success, press SW4 button and reset the board, and try to erase again)

   c. Select the bin data file, set the '.bin/Erase Start' address to 0x10000000, then click 'Program Device'
Note: Please use above way to program the binary file built by armgcc tool chain to external flash. 
      For IAR, KEIL, MCUXpresso IDE, you can use the IDE tool to program the external flash.  
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
SHELL build: Sep 27 2020
Copyright  2020  NXP

LOG SHELL>> installbackend debugconsole
LOG SHELL>> log fatal
           0: FATAL log_main.c:83:This is "fatal" log message

LOG SHELL>> log error
           0: ERROR log_main.c:87:This is "error" log message

LOG SHELL>> log warning
           0: WARN  log_main.c:91:This is "warning" log message

LOG SHELL>> log info
           0: INFO  log_main.c:95:This is "info" log message

LOG SHELL>> log debug
           0: DEBUG log_main.c:99:This is "debug" log message

LOG SHELL>> log trace
           0: TRACE log_main.c:103:This is "trace" log message

LOG SHELL>> log test
           0: ERROR log_main.c:107:The input arguement "test" is not valid

LOG SHELL>> uninstallbackend debugconsole
LOG SHELL>> 
~~~~~~~~~~~~~~~~~~~~~
Note: The shell information "SHELL build: Sep 27 2020" may be different, which depends on the compile date.
