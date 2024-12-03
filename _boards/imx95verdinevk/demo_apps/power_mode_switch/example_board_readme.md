Hardware requirements
=====================
- USB Type-C cable
- IMX95LPD5-verdin board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer
- Headphone

Board settings
============
No special settings are required.

Prepare the Demo
================
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW301 to power on the board.
2.  Connect a USB Type-C cable between the host PC and the J1401 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
Mcore debug console shows the menu to command the MCU to the target power mode. The target power mode can be wakeup by LPTMR and LPUART and SM wakeup message.
How to enter system suspend mode(such as M7 use sm as wakeup source): In linux console use command "echo mem > /sys/power/state" to suspend A55, then select SUSPEND mode in Mcore debug console and input "S" into select SM as wakeup source, input "idle" command in M33 console.
How to escape system suspend mode(such as M7 use sm as wakeup source): press enter in SM console, then input "wake" command to wakeup M7 and A55. Or press ONOFF button in board.
NOTE:
      1. M7 support LPTMR/LPUART/SM interface(wakeup command in sm consle or ONOFF button in board) wakeup source.
      2. Option 'P' will suspend A55 core.
      3. Please ensure the A55 already in SUSPEND mode, then press "W" button in Mcore debug console. A55 core will be wakeup.

The log below shows the output of the power mode switch demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Start SRTM communication
Task 1 is working now

####################  Power Mode Switch Task ####################

    Build Time: Apr 25 2024--11:25:33
    Core Clock: 800000000Hz

Select the desired operation

Press  A to enter: Normal RUN mode
Press  B to enter: WAIT mode
Press  C to enter: STOP mode
Press  D to enter: SUSPEND mode
Press  P to suspend A55 core
Press  W to wakeup A55 core

Waiting for power mode select..

Handle Peer Core Linkup
