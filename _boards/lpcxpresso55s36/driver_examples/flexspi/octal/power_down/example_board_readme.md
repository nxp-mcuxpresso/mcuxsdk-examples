Hardware requirements
=====================
- Micro USB cable
- LPCXpresso55S36 board
- Personal Computer

Board settings
============
Connect pin 1-2 of JP41,JP42,JP43,JP44,JP45,JP62,JP64 and JP65.
Close JP63,JP66,JP67,JP68 and JP69.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J1) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXSPI example enter power down!
System will wakeup at 5s later

FLEXSPI example wakeup from power down!
Vendor ID: 0xc2
Erasing Serial NOR over FlexSPI...
Erase data - successfully. 
Program data - successfully.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Note: This example doesn't need to connect external octal flash with FLEXSPI.