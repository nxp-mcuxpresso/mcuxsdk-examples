Hardware requirements
=====================
- Mini/micro USB cable
- MCXW23-EVK Board
- Personal Computer

Board settings
============
1. Pin J21_2 is used as a CLKOUT signal. Connect this pin to an Oscilloscope for viewing the signal.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the EVK board J33.
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
The demo sets the MicroTick Timer as a wake up source and puts the device in sleep mode. 
The MicroTick timer wakes up the device.

 After wake up：

(1)The red LED on the board blinking. 
(2)CLKOUT signal changing can be seen on pin J21_2. And the frequency of  output signal will double.

If the demo run successfully, you will see the log from debug console:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Utick wakeup demo start...
Wakeup from sleep mode...

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~