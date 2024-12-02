Toolchain supported
---------------------
- IAR embedded Workbench 8.50.5
- Keil MDK 5.31
- GCC ARM Embedded  9.2.1
- MCUXpresso 11.2.0

Hardware Requirements
------------------------
- Micro USB cable
- LPCXpresso860MAX board
- Personal Computer

Board settings
==============
Remove the R51. Connect the adjustable input voltage to JP2-1.

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J4) on the target board.
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the BOD demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BOD INTERRUPT EXAMPLE.
Please adjust input voltage low than 2.66V to trigger BOD interrupt.

BOD interrupt occurred, input voltage is low than 2.66V.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
=====================

Note:When the BOD demo is started, the analog voltage input of the JP2_1 pin should be 3.3v. After running the demo, 
adjust the analog voltage input according to the prompts to trigger the BOD interrupt.