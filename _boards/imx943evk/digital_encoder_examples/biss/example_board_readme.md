Hardware requirements
=====================
- Mini/micro USB cable
- i.MX943-EVK board
- Personal Computer
- BiSS sensor

Board settings
==============

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the MCU-LINK USB port on the target board.
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
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BiSS test example.
This example use one board as BiSS master and connect to the BiSS encoder.
Please make sure you make the correct line connection. Basically, the connection is:
   MA       --   BISS Clock Line Output
   MO       --   BISS Data Line Output
   SL       --   BISS Data Line Input
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
