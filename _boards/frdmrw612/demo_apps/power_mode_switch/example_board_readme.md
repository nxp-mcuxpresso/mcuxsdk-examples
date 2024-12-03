Hardware requirements
=====================
- Micro USB cable
- FRDM-RW612 board
- Personal Computer

Board settings
============
No special settings

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J10) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When running the demo, the debug console shows the menu to command the MCU to the target power mode (PM0 - Active, PM1 - Idle, PM2 - Standby, PM3 - Sleep, PM4 - Deep Sleep).
NOTE: Only input when the demo asks for input. Input entered at any other time might cause the debug console to overflow and receive the wrong input value.

Here's the initial menu when running successfully.
~~~~~~~~~~~~~~~~~~~~~
MCU wakeup source 0x0...

####################  Power Mode Switch ####################

    Build Time: Mar  2 2022--10:11:47
    Core Clock: 260000000Hz

Select the desired operation

Press 0 for enter: PM0 - Active
Press 1 for enter: PM1 - Idle
Press 2 for enter: PM2 - Standby
Press 3 for enter: PM3 - Sleep
Press 4 for enter: PM4 - Deep Sleep

Waiting for power mode select..

1
Select the wake up source:
Press T for RTC.
Press 1 for wakeup pin1(SW2).
Press U for UART wakeup.

Waiting for key press..

t
Select the wake up timeout in seconds.
The allowed range is 1s ~ 99s.
Eg. enter 05 to wake up in 5 seconds.

Waiting for input timeout value...

03
RTC wake up after 3 seconds.
Woken up by RTC
Exit from power mode 1

####################  Power Mode Switch ####################

    Build Time: Mar  2 2022--10:11:47
    Core Clock: 260000000Hz

Select the desired operation

Press 0 for enter: PM0 - Active
Press 1 for enter: PM1 - Idle
Press 2 for enter: PM2 - Standby
Press 3 for enter: PM3 - Sleep
Press 4 for enter: PM4 - Deep Sleep

Waiting for power mode select..

3
Select the wake up source:
Press T for RTC.
Press 1 for wakeup pin1(SW2).

Waiting for key press..

1
Push wakeup PIN1 to wake up.

MCU wakeup source 0x0...

####################  Power Mode Switch ####################

    Build Time: Mar  2 2022--10:11:47
    Core Clock: 260000000Hz

Select the desired operation

Press 0 for enter: PM0 - Active
Press 1 for enter: PM1 - Idle
Press 2 for enter: PM2 - Standby
Press 3 for enter: PM3 - Sleep
Press 4 for enter: PM4 - Deep Sleep

Waiting for power mode select..

4
Select the wake up source:
Press T for RTC.
Press 1 for wakeup pin1(SW2).

Waiting for key press..

t
Select the wake up timeout in seconds.
The allowed range is 1s ~ 99s.
Eg. enter 05 to wake up in 5 seconds.

Waiting for input timeout value...

02
RTC wake up after 2 seconds.

MCU wakeup source 0x0...

####################  Power Mode Switch ####################

    Build Time: Mar  2 2022--10:11:47
    Core Clock: 260000000Hz

Select the desired operation

Press 0 for enter: PM0 - Active
Press 1 for enter: PM1 - Idle
Press 2 for enter: PM2 - Standby
Press 3 for enter: PM3 - Sleep
Press 4 for enter: PM4 - Deep Sleep

Waiting for power mode select..
~~~~~~~~~~~~~~~~~~~~~
