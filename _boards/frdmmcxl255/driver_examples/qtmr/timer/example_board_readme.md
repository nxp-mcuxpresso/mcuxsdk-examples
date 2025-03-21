Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
============
Make sure you have jumpers SJ1 and SJ2 set to
- 2-1 for the main core (cm33)
- 2-3 for AON core (cm0+)

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
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*********QUADTIMER EXAMPLE START*********
****Timer use-case, 50 millisecond tick.****

 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
****Chain Timer use-case, 10 second tick.****

 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
*********QUADTIMER EXAMPLE END.*********
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
