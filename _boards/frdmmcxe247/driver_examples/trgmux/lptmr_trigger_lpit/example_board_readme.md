Overview
========
The lptmr_trigger_lpit project is a simple example of the SDK LPIT, LPTMR and TRGMUX driver which shows how to 
use the LPTMR and TRGMUX to generate a LPIT trigger. It sets up the LPTMR hardware block to generate the trigger every 0.5 second,
and the LPIT counter will load on the first trigger rising edge and then decrement down to zero on each trigger rising edge.
A message would be printed on the serial terminal.

Hardware requirements
=====================
- USB-C cable
- FRDM-MCXE247 board
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
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~
Example Starts!
LPIT interrupt No.1 
LPIT interrupt No.2 
LPIT interrupt No.3 
LPIT interrupt No.4 
LPIT interrupt No.5 
LPIT interrupt No.6 
LPIT interrupt No.7 
LPIT interrupt No.8 
LPIT interrupt No.9 
LPIT interrupt No.10 
LPIT interrupt No.11 
LPIT interrupt No.12 
LPIT interrupt No.13 
......
~~~~~~~~~~~~~~~~~~~~~~~
