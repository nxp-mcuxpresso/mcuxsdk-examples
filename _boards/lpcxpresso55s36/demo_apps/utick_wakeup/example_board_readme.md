Hardware requirements
=====================
- Micro USB cable
- LPCXpresso55S36 board
- Personal Computer

Board settings
============
1. J132-4(PIO0_16) is used as a CLKOUT signal. Connect this pin to an Oscilloscope for viewing the signal. 

Running the demo
================
The demo sets the MicroTick Timer as a wake up source and puts the device in sleep mode. 
The MicroTick timer wakes up the device.

 After wake up：

(1)the red LED on the board blinking. 
(2)CLKOUT signal changing can be seen on J132-4. And the frequency of output signal will double.

If the demo run successfully, you will see the log from debug console:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Utick wakeup demo start...
Wakeup from sleep mode...

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~