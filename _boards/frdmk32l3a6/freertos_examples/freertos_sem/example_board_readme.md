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
Producer_task created.

Consumer_task 0 created.

Consumer_task 1 created.

Consumer_task 2 created.

Consumer number: 0

Consumer 0 accepted item.

Consumer number: 1

Consumer number: 2

Producer released item.

Consumer 0 accepted item.

Producer released item.

Consumer 1 accepted item.

Producer released item.

Consumer 2 accepted item.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~