Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
============
1. Connect pin 9(RX) and pin 11(TX) of J8 to a serial port adapter to enable the UART function of the AON domain.
2. The data of power consumption is measured on JP4(VDD_BAT).

Note: For boards with the schematic SCH-95308 REV E, ensure that SJ20 is connected to pins 2 and 3, 
and SJ6 is connected to pins 2 and 3. This configuration allows simultaneous use of LPUART and AON_UART.


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
Reset Reasons: 6
Copy Secondary core image to address: 0xa1000000, size: 11428
Start to communication with secondary core...

Select the desired operation...
        Press A to start transition: Active --> Sleep --> Active
        Press B to start transition: Active --> Deep Sleep --> Active
        Press C to start transition: Active --> Power Down1 --> Active
        Press D to start transition: Active --> Power Down2 --> Active
        Press E to start transition: Active --> Deep Power Down1 --> Active
        Press F to start transition: Active --> Deep Power Down1 --> Deep Power Down2 --> Deep Power Down1 --> Active
        Press G to start transition: Active --> Deep Power Down1 --> Deep Power Down2 --> Active
        Press H to start transition: Active --> Deep Power Down2 --> Active
        Press I to start transition: Active --> Deep Power Down2 --> Deep Power Down1 --> Active
        Press J to start transition: Active --> Deep Power Down3 --> Active
        Press K to start transition: Active --> Shut Down --> Active

Waiting for selecting power transition...

~~~~~~~~~~~~~~~~~~~~~
