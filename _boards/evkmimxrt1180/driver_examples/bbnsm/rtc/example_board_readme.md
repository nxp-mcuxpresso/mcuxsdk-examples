Hardware requirements
=====================
- Micro USB cable
- MIMXRT1180-EVK board
- Personal Computer

Board settings
============
No special settings are required.

**Please note this application can't support running with Linux BSP!**

Prepare the Demo
===============
1. Connect 5V power supply and J-Link Debug Probe to the board and power on the board.
2. Connect a micro USB cable between the host PC and the J53 USB port on the target board.
3. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4. Download the program to the target board.
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~
BBNSM RTC example: set up time to wake up an alarm
Current datetime: 2024-12-25 19:00:00
Please input the number of seconds to wait for the alarm
The seconds must be a positive value
~~~~~~~~~~~~~~~~~~~~~~~

After a number of seconds is entered (for example 5) and confirmed with the Enter key, the alarm
time is shown and the alarm fires when the RTC reaches it:
~~~~~~~~~~~~~~~~~~~~~~~~
Alarm will occur at: 2024-12-25 19:00:05

 Alarm occurs !!!!
~~~~~~~~~~~~~~~~~~~~~~~
