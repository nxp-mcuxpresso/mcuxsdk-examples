Hardware requirements
=====================
- Type-C USB cable
- KW47-EVK Board
- Personal Computer

Board settings
==============


Prepare the Demo
================
1. Connect a USB cable between the host PC and the EVK board J14.
2. Download the program to the target board.
    * The program may require to be loaded twice without a power-cycle
      in-between loads.
3. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the hello world multicore demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Secondary core started

Initialization of the S200 was first done by NBU core

ALL TASKS COMPLETED, CHECKING SUCCESS RATES :
  Main core successfully completed 25 tasks out of 25
  NBU  core successfully completed 25 tasks out of 25
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
