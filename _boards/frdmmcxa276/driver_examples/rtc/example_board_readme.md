Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA276 board
- Personal Computer

Board settings
==============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.

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
================
When the example runs successfully, you can see the similar information from the terminal as below.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
RTC example: set up time to wake up an alarm
Current datetime: 2014-12-25 19:00:00
Please input the number of second to wait for alarm
The second must be positive value
Alarm will occur at: 2014-12-25 19:01:11

Alarm occurs !!!! Current datetime: 2014-12-25 19:01:12
Please input the number of second to wait for alarm
The second must be positive value
...............................................
...............................................
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
