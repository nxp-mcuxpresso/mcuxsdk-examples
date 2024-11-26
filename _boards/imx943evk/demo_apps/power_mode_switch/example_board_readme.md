Hardware requirements
=====================
- Micro USB cable
- IMX943-EVK board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW301 to power on the board.
2.  Connect a micro USB cable between the host PC and the J1401 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example.
6.  Append "clk_ignore_unused" in u-boot "mmcargs" env, before booting linux.

Running the demo
================
Mcore debug console shows the menu to command the MCU to the target power mode. The target power mode can be wakeup by LPTMR.
SYSTEM SLEEP flow: In linux console use command "echo mem > /sys/power/state" to suspend A55, then select SUSPEND mode in Mcore debug console. The STBY LED on board will light with RED color.
NOTE: M33 wakeup A55 by using MU interrupt "GCR[GIR1]". Please ensure the A55 already in SUSPEND mode, then press "W" button in Mcore debug console. Or you will meet call trace info in linux.

The log below shows the output of the power mode switch demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Start SRTM communication
Task 1 is working now

####################  Power Mode Switch Task ####################

    Build Time: Dec  2 2022--15:15:19
    Core Clock: 200000000Hz

Select the desired operation

Press  A to enter: Normal RUN mode
Press  B to enter: WAIT mode
Press  C to enter: STOP mode
Press  D to enter: SUSPEND mode
Press  W to wakeup A55 core

Waiting for power mode select..
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
