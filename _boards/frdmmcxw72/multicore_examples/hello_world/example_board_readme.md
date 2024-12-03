Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW72 Board
- Personal Computer

Board settings
==============
For FRDM-MCXW72 board, connect J1 pin 7 to J4 pin 4 to allow LED2_BLUE
control from the secondary core application.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the board J10.
2. Download the program to the target board.
3. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the hello world multicore demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Starting Secondary core.

Hello World from the Primary Core!

Starting Secondary core.
The secondary core application has been started.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
