Hardware requirements
=====================
- Mini/micro USB cable
- KW45B41Z-EVK Board
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
RTC example: set up time to wake up an alarm
Current datetime: 2014-12-25 19:00:00
Please input the number of second to wait for alarm
The second must be positive value
Alarm will occur at: 2014-12-25 19:01:11

Alarm occurs !!!! Current datetime: 2014-12-25 19:01:12
Please input the number of second to wait for alarm
The second must be positive value
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

