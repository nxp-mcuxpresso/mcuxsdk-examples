Hardware requirements
===================
- USB-C cable
- FRDM-MCXE247 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the MCU-LINK USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When running the demo, the debug console shows the menu to tell how to set MCU
to target power mode.

This Power Mode Switch demo directly controls the selected MCU power mode from the menu flow. Use the Power Manager demo
when an application needs notification callbacks before entering and after exiting a power mode.

NOTE: Only input when the demo asks to input, input at other time might make the
debug console overflow and get wrong result.
~~~~~~~~~~~~~~~~~~~~~
####################  Power Mode Switch Demo ####################

    Core Clock = 80000000Hz
    Power mode: RUN

Select the desired operation

Press  A for enter: RUN      - Normal RUN mode
Press  B for enter: STOP     - Stop mode
Press  C for enter: VLPR     - Very Low Power Run mode
Press  D for enter: VLPS     - Very Low Power Stop mode
Press  E for enter: HSRUN    - High Speed RUN mode

Waiting for power mode select..
~~~~~~~~~~~~~~~~~~~~~

When VLPS mode is selected and the configured wakeup event occurs, the debug console prints:

~~~~~~~~~~~~~~~~~~~~~
Entered VLPS mode and woke up.

Next loop
~~~~~~~~~~~~~~~~~~~~~
