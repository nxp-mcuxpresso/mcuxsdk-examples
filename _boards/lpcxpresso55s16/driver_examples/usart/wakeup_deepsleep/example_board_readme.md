Hardware requirements
===================
- Micro USB cable
- LPCXpresso55s16 board
- Personal Computer

Board settings
============
No special is needed.

Prepare the Demo
===============
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J1) on the target board.
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
When the demo runs successfully, the log would be seen on the CMSIS DAP terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Usart waking up soc from deep sleep example, please note USART can only work at 9600 baudrate in deep sleep mode

Press 1 to enter deep sleep
Press any other key to wake up soc
/* Type in '1' into UART terminal */
Received 1
Entering deep sleep mode, please change the baudrate setting of your local terminal to 9600
/* Type in '2' into UART terminal */
Received 2
Waking up from deep sleep, please change the baudrate setting of your local terminal back to 115200
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
