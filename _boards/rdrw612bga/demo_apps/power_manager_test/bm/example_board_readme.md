Hardware requirements
=====================
- Micro USB cable
- RD-RW61X-BGA board
- Personal Computer

Board settings
============
Ensure R148 is removed.
U38 DIP 1,2,3,4 all off
HD12 1-2, 3-4 connected
JP31 1-2 connected
JP34 2-3 connected
JP39 2-3 connected

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J7) on the board
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
The log below shows the output of the power manager test demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MCU wakeup source 0x0...

Power Manager Test.

Please select the desired power mode:
        Press A to enter: PM0 - Active
        Press B to enter: PM1 - Idle
        Press C to enter: PM2 - Standby
        Press D to enter: PM3 - Sleep
        Press E to enter: PM4 - Deep Sleep

Waiting for power mode select...

Select the wake up timeout in seconds.
The allowed range is 1s ~ 9s.
Eg. enter 5 to wake up in 5 seconds.

Waiting for input timeout value...

2
Will wakeup in 2 seconds.
De-init UART.
Re-init UART.
Woken up by RTC

Next Loop

Please select the desired power mode:
        Press A to enter: PM0 - Active
        Press B to enter: PM1 - Idle
        Press C to enter: PM2 - Standby
        Press D to enter: PM3 - Sleep
        Press E to enter: PM4 - Deep Sleep

Waiting for power mode select...

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
