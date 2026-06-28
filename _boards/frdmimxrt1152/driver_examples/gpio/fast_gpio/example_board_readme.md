Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMXRT1152 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GPIO Driver example
This example is intended to run in debug/release configuration, 
with all code and data in TCM, to show the potential of fast gpio

Core Clock = 798MHz, Bus Clock = 240MHz
Loop num = 1000000
  Normal GPIO cycles 3 output takes 175ms
  Normal GPIO cycles 4 output takes 233ms
  Normal GPIO input takes 77ms
  Fast GPIO cycles 3 output takes 16ms
  Fast GPIO cycles 4 output takes 21ms
  Fast GPIO input takes 23ms
Fast GPIO IRQ occurred!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note: This example is intended to run in debug/release configuration, with all code and data in TCM, to show the potential of fast gpio
Note: The xxms in terminal window, is from debug configuration. Differernt build configuration leads slight change for these numbers.
