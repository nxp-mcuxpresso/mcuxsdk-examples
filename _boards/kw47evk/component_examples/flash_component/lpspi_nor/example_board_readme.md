Hardware requirements
=====================
- Type-C USB cable
- KW47-EVK Board
- Personal Computer

Board settings
============
- Put a jumper on JP31

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the EVK board J14.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Note:
To make the external NOR flash run under the maximum 24MHz, please update the value of the macro "BOARD_LPSPI_NOR_BAUDRATE" from 4000000U to 24000000U in the board.h.

Running the demo
===============
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
***NOR Flash Component Demo Start!***

***NOR Flash Erase Chip Start!***


***NOR Flash Page 0 Read/Write Success!***


***NOR Flash Page 1 Read/Write Success!***


***NOR Flash Page 2 Read/Write Success!***


***NOR Flash Page 3 Read/Write Success!***


***NOR Flash Page 4 Read/Write Success!***


***NOR Flash Page 5 Read/Write Success!***


***NOR Flash Page 6 Read/Write Success!***


***NOR Flash Page 7 Read/Write Success!***


***NOR Flash Page 8 Read/Write Success!***


***NOR Flash Page 9 Read/Write Success!***


***NOR Flash Page 10 Read/Write Success!***


***NOR Flash Page 11 Read/Write Success!***


***NOR Flash Page 12 Read/Write Success!***


***NOR Flash Page 13 Read/Write Success!***


***NOR Flash Page 14 Read/Write Success!***


***NOR Flash Page 15 Read/Write Success!***

***NOR Flash All Pages Read/Write Success!***
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~