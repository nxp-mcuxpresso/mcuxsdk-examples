Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMXRT1152 board
- Personal Computer

Board settings
============
Matrix and Jumper settings for KPP:

row2  <----->   J11 #8
row3  <----->   J11 #12
col2  <----->   J11 #10
col3  <----->   J11 #6

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

 Key SW1 was released.

 Key SW3 was released.

 All keys released.

 This was a long press.

   ......

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
