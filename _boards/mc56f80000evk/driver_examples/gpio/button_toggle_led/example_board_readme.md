Hardware requirements
=====================
- Micro USB cable
- MC56F80000-EVK board
- Personal Computer

Board hardware common setting
=============================
1. Power setting:
   > Connect J8 pin2 and pin3
   > Connect J6 pin1 and pin2
   > Connect J5 pin1 and pin2
2. Reset pin setting:
   > Connect J11 pin2 and pin3
3. Debugger setting:
   > Connect J13 pin 1-2, 3-4, 5-6, 7-8

Board settings
==============
- Connect J17 pin 1-2.

Prepare the Demo
================
1.  Connect USB cable between the host PC and the J12 USB port on the target board. It setups Multilink and COM port in PC device manager.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with Multilink debug configuration.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GPIO Driver example
Press the button SW2 to toggle the LED
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Press the button SW2 and you will find the RED LED is toggled with log output.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Button press event detected....
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
