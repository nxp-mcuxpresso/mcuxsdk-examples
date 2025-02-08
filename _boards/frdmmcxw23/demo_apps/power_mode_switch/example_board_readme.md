Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW23 Board
- Personal Computer

Board settings
============
No special settings are required.

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
1.The demo starts with a print of the boot reason:
====================================================
!!!MCXW23 BOOTED!!!
Boot reason:
DC/DC mode: buck mode (HV)
DC/DC startup duration: 139 us
====================================================

2.Then it presents a menu with multiple selections; power states, device wake up from different sources, device reset, and FROs calibration :
================= SELECT FROM MENU =================

a. Get power library version
b. Go to sleep. Wakeup from RTC timer
c. Go to sleep. Wakeup from OS timer
d. Go to sleep. Wakeup from Ctimer
e. Go to sleep. Wakeup from pin toggle (PINT)
f. Go to sleep. Wakeup from pin toggle (GINT)
g. Go to deep sleep. Wakeup from RTC timer
h. Go to deep sleep. Wakeup from OS timer
i. Go to deep sleep. Wakeup from Ctimer
j. Go to deep sleep. Wakeup from pin toggle (PINT)
k. Go to deep sleep. Wakeup from pin toggle (GINT)
l. Go to power down. Wakeup from RTC timer
m. Go to power down. Wakeup from OS timer
n. Go to deep power down. Wakeup from RTC timer
o. Go to deep power down. Wakeup from OS timer
p. Go to deep power down. Wakeup from pin toggle (always on wakeup pin 21)
q. Go to power off. Wakeup from pin toggle (always on wakeup pin 21)
r. Reset device
s. Calibrate FROs
>
For each option, press the corresponding letter and then press OK.
