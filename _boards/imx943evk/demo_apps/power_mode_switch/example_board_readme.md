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
How to enter system suspend mode(such as Mcore use sm as wakeup source): In linux console use command "echo mem > /sys/power/state" to suspend A55, then select SUSPEND mode in Mcore debug console and input "S" into select SM as wakeup source, input "idle" command in M33 console. The STBY LED on board will light.
How to escape system suspend mode(such as M7 use sm as wakeup source): press enter in SM console, then input "wake" command to wakeup Mcore and A55. Or press ONOFF button in board.
NOTE:
      1. Mcore support TIMER/LPUART/SM interface(wakeup command in sm consle or ONOFF button in board) wakeup source.
         1.1 Due to hardware clock source limitations which the clock sources supported by TPM will be turned off when the system is suspended, we only support TIMER wakeup on CM7_CORE0 in system suspend mode, do not support TIMER wakeup on CM33_CORE1/CM7_CORE1 in system suspend mode.
      2. Option 'P' will suspend A55 core.
      3. Please ensure the A55 already in SUSPEND mode, then press "W" button in Mcore debug console. A55 core will be wakeup.
      4. Support lm core_id suspend and lm core_id wake command in system manager console, lm info get core_id:
         4.1 >$ lm info
             000: SM   = on
             001: M33S = on
             002: M7   = on
             003: M71  = on
             004: AP   = on (default)
      5. Only M7_core0 and M7_core1 can use option 'P' 'W', reserved for cm33_core1.

The log below shows the output of the power mode switch demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Start SRTM communication
Task 1 is working now

####################  Power Mode Switch Task ####################

    Build Time: Feb 20 2025--07:59:03
    Core Clock: 266666666Hz

Select the desired operation

Press  A to enter: Normal RUN mode
Press  B to enter: WAIT mode
Press  C to enter: STOP mode
Press  D to enter: SUSPEND mode
Press  P to suspend A55 core
Press  W to wakeup A55 core

Waiting for power mode select..
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
