Hardware requirements
===================
- Mini/micro USB cable
- MCX-N5XX-EVK Board
- Personal Computer

Board settings
============
1. To wakeup the chip via SW4, please populate R198.
2. To measure current consumption via MCULink, please remove JP29.

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
When running the demo, the debug console shows the menu to command the MCU to the target power mode.

NOTE: 
1.Only input when the demo asks for input. Input entered at any other time might cause the debug console to overflow
and receive the wrong input value.
2. Wake-up from Deep Power Down mode through wakeup reset that will reset the program("Normal Boot" will be printed).
3. The signal output from J2_7 can be used to observe entry/exit of DeepSleep, PowerDown, and Deep Power Down mode.
~~~~~~~~~~~~~~~~~~~~~
Normal Boot.

###########################    Power Mode Switch Demo    ###########################
    Core Clock = 48000000Hz
    Power mode: Active

Select the desired operation

        Press A to enter: Active mode
        Press B to enter: Sleep mode
        Press C to enter: DeepSleep mode
        Press D to enter: PowerDown mode
        Press E to enter: DeepPowerDown mode

Waiting for power mode select...

~~~~~~~~~~~~~~~~~~~~~
