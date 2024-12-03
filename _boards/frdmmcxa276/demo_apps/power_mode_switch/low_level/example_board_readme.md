Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA276 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
when running the demo, the debug console shows the menu to command the MCU to the target power mode.

NOTE: 
1.Only input when the demo asks for input. Input entered at any other time might cause the debug console to overflow
and receive the wrong input value.
2. Wake-up from Deep Power Down mode through wakeup reset that will reset the program("Normal Boot" will be printed).
~~~~~~~~~~~~~~~~~~~~~
Normal Boot.

###########################    Power Mode Switch Demo    ###########################
    Core Clock = 180000000Hz
    Power mode: Active

Select the desired operation

        Press A to enter: Active mode
        Press B to enter: Sleep mode
        Press C to enter: DeepSleep mode
        Press D to enter: PowerDown mode
        Press E to enter: DeepPowerDown mode

Waiting for power mode select...

~~~~~~~~~~~~~~~~~~~~~
