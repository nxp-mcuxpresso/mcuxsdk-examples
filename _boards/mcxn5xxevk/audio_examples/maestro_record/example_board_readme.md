Hardware requirements
=====================
- Mini/micro USB cable
- MCX-N5XX-EVK board
- Personal Computer
- headphones with 3.5 mm stereo jack
- micro SD card for file output

Board settings
============
Insert the micro SD card into the card slot
Short JP7 2-3, JP8 2-3, JP10 2-3, JP11 2-3

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect headphones to Audio HP / Line-Out connector.
2.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
3.  Open a serial terminal with the following settings :
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

# Note
======
Some VIT models can't fit into memory. In order to free some space it is necessary to disable SD card handling.
 - remove SD_ENABLED and STREAMER_ENABLE_FILE_SINK symbols from project settings

