Hardware requirements
===================
- Type-C USB cable
- FRDM-MCXW71 Board
- Personal Computer

Board settings
============
1. To wakeup the chip via VBAT, please press the SW5, and the falling edge will wakeup the chip.
2. The default connection of SH105 is 1-2. To test PowerSwitchOff mode, please short 2-3 of SH105.
   To test other modes, please short 1-2 of SH105.

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

NOTE: Only input when the demo asks for input. Input entered at any other time might cause the debug console to overflow
and receive the wrong input value.
~~~~~~~~~~~~~~~~~~~~~
###########################    Power Mode Switch Demo    ###########################
    Core Clock = 96000000Hz
    Power mode: Active

Please Select the desired power mode:

        Press A to enter Active mode!
        Press B to enter Sleep1 mode!
        Press C to enter DeepSleep1 mode!
        Press D to enter DeepSleep2 mode!
        Press E to enter DeepSleep3 mode!
        Press F to enter DeepSleep4 mode!
        Press G to enter PowerDown1 mode!
        Press H to enter PowerDown2 mode!
        Press I to enter PowerDown3 mode!
        Press J to enter PowerDown4 mode!
        Press K to enter DeepPowerDown1 mode!
        Press L to enter DeepPowerDown2 mode!
        Press M to enter PowerSwitchOff mode!

        Waiting for power mode select...


~~~~~~~~~~~~~~~~~~~~~
