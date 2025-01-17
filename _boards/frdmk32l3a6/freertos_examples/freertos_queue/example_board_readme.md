Hardware requirements
=====================
- Mini USB cable
- FRDM-K32L3A6 board
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for JLink serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
The following message shows in the terminal if the example runs successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Log 0: Task1 Message 0

Log 1: Task2 Message 0

Log 2: Task1 Message 1

Log 3: Task2 Message 1

Log 4: Task1 Message 2

Log 5: Task2 Message 2

Log 6: Task1 Message 3

Log 7: Task2 Message 3

Log 8: Task1 Message 4

Log 9: Task2 Message 4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~