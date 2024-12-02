
Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso55s28 board
- Personal Computer
- ROTARY Y Click board(could be purchased from the website: https://www.mikroe.com/rotary-y-click)

Board settings
==============
The jumper setting:
    Default jumpers configuration does not work,  you will need to add JP20 and JP21 (JP22 optional for ADC use)

Prepare the Demo
================
1.  Connect the ROTARY Y Click board and EVK board.
2.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (P6) on the board
3.  Open a serial terminal with the following settings:
    - 512000 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo. 


Running the demo
================
Four LEDs will be lit. 
Rotate the handle on the ROTARY Y Click.
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
QEI demo.
When the QEI state changes,the counter and direction will be got.
Push the WAKEUP button on LPCXpresso55s28 board to change the LED display model.

Counter: 1 
Direction:Clockwise 

Counter: 2 
Direction:Clockwise 

Counter: 3 
Direction:Clockwise 

Counter: 4 
Direction:Clockwise
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~