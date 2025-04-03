Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect a Type-C USB cable between the host PC and the MCU-Link port(J16) on the target board.
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

~~~~~~~~~~~~~~~~~~~~~
###########################  Power Mode Switch Demo Primary Core Boot  ###########################
Normal Boot......
Core Clock Frequency: 96000000
Copy Secondary core image to address: 0xa1000000, size: 7720

Select the desired operation...
        Press A to enter: Active mode
        Press B to enter: Sleep mode
        Press C to enter: Deep Sleep mode
        Press D to enter: Power Down1 mode
        Press E to enter: Power Down2 mode
        Press F to enter: Deep Power Down1 mode
        Press G to enter: Deep Power Down2 mode
        Press H to enter: Deep Power Down3 mode
        Press I to enter: Shut Down mode

Waiting for power mode select...

~~~~~~~~~~~~~~~~~~~~~
