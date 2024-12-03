Hardware requirements
=====================
- Type-C USB cable
- MCX-W72-EVK Board
- Personal Computer

Board settings
==============
For KW-MCXW-EVK-MB RevB board, remove jumper on JP4 and manually connect JP4 pin 1 to J4 pin 7 (Arduino A2 pin)
to allow LED2_BLUE control from the secondary core application.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the EVK board J14.
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
