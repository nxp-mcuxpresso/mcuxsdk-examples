Hardware requirements
=====================
- Type-C USB cable
- KW47-EVK Board
- Personal Computer

Board settings
============
No special is needed.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, you will see the similar information from the terminal shown as below.
Detail prints for each channel depends on by the total numbs of chained channels and the order of 
interrupt flag handler process for each channel. 

~~~~~~~~~~~~~~~~~~~~~
Starting channel No.0 ...
Starting channel No.1 ...
Starting channel No.2 ...
Starting channel No.3 ...

 Channel No.0 interrupt is occurred !
 Channel No.0 interrupt is occurred !
 Channel No.0 interrupt is occurred !
 Channel No.0 interrupt is occurred !
 Channel No.0 interrupt is occurred !

 Channel No.1 Chained with Channel No.0 interrupt is occurred !
......................................
 Channel No.1 Chained with Channel No.0 interrupt is occurred !
......................................
 Channel No.1 Chained with Channel No.0 interrupt is occurred !
......................................
 Channel No.1 Chained with Channel No.0 interrupt is occurred !
......................................
 Channel No.1 Chained with Channel No.0 interrupt is occurred !

 Channel No.2 Chained with Channel No.1 interrupt is occurred !
......................................
......................................
 Channel No.2 Chained with Channel No.1 interrupt is occurred !
......................................
......................................
 Channel No.2 Chained with Channel No.1 interrupt is occurred !
......................................
......................................
 Channel No.2 Chained with Channel No.1 interrupt is occurred !
......................................
......................................
 Channel No.3 Chained with Channel No.2 interrupt is occurred !
 .....................................
 .....................................
 .....................................

~~~~~~~~~~~~~~~~~~~~~
