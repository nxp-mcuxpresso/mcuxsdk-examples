Hardware requirements
=====================
- Type-C USB cable
- KW47-LOC Board
- Personal Computer

Board settings
==============
For KW47-LOC board, remove jumper on JP23 and JP24, manually connect JP23 pin 1 to JP24 pin 1
to allow LED2_BLUE control from the secondary core application.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the LOC board J3.
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
