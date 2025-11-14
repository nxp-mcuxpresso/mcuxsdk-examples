Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW23 Board
- Personal Computer

Board settings
============
Probe JP12 to measure power consumption in different power modes:

| Power Mode | Current Consumption |
|------------|---------------------|
| Sleep | 304uA |
| Deep Sleep | 5.57uA |
| Power Down | 2.28uA |
| Deep Power Down | 570nA |
| Power Off | 15nA |

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the FRDM board J10.
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

The log below shows the output of the power mode switch demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Normal Boot.
Calibrating RTC Clock Dividers...

Please select the desired power mode:
        Press A to enter: Sleep
        Press B to enter: Deep Sleep
        Press C to enter: Power Down(CPU RET)
        Press D to enter: Deep Power Down
        Press E to enter: Power Off

Waiting for power mode select...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
