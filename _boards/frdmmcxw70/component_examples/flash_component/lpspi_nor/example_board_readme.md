Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW70 Board
- Personal Computer

Board settings
============
- U30 and U12C affects the LPSPI1_SIN signal, causing it to remain at a low level.
  Note: Short JP6 to disable U30 before running this example.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the FRDM board J28.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
***NOR Flash Component Demo Start!***


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