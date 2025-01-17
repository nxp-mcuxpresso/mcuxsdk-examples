Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW71 Board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect the micro and mini USB cable between the PC host and the USB ports on the board.
2. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
The following lines are printed to the serial terminal when the demo program is executed.

~~~~~~~~~~~~~~~~~~~~~
RTC Demo running...

Please choose the sub demo to run:
1) Get current date time.
2) Set current date time.
3) Alarm trigger show.
4) Second interrupt show (demo for 20s).

Select:
~~~~~~~~~~~~~~~~~~~~~

1. Get current date time:

This command gets the current date time in the format of YYYY-MM-DD HH:MM:SS.
~~~~~~~~~~~~~~~~~~~~~
Current datetime: 2015-11-11 11:11:11
~~~~~~~~~~~~~~~~~~~~~

2. Set current date time

Sets the date time as YYYY-MM-DD HH:MM:SS.
~~~~~~~~~~~~~~~~~~~~~
Input date time like: "2010-10-10 10:10:10"
2009-10-10 10:10:10
~~~~~~~~~~~~~~~~~~~~~

3. Show the alarm function

Sets the alarm time: 1~9 seconds from the time you input and waits for the alarm event arrival.
~~~~~~~~~~~~~~~~~~~~~
Input the alarm seconds from the current moment (1s~9s):3
Triggered Alarm: 10:10:31
~~~~~~~~~~~~~~~~~~~~~

4. Seconds interrupt show

Shows the digital time animation by seconds-interrupt function.
~~~~~~~~~~~~~~~~~~~~~
10:10:51
10:10:51
...
10:11:11
~~~~~~~~~~~~~~~~~~~~~
