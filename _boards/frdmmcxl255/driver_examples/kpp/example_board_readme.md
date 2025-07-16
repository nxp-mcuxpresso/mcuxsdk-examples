Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
============
Matrix and Jumper settings for KPP:

row1  <----->   J8_5  (P0_13)
row0  <----->   J8_7  (P0_12)
col1  <----->   J8_4  (P0_3)
col0  <----->   J8_8  (P0_2)

Make sure you have solder bridges SJ1 and SJ2 set to
- 2-1 for the main core (cm33)

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Connect the 2*2 matrix to the Jumpers mentioned above. 
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============

When you press any key on matrix,  the log will show the right key you have pressed.
If the press is long, it will add the long press mention.
The log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 KPP Driver Example Start.
  
 Key SW1 was pressed.

 Key SW3 was pressed.

 This was a long press.

   ......

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
