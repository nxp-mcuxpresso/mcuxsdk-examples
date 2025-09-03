Hardware requirements
=====================
- FRDMMCXA266 board
- FRDM-MC-LVPMSM board
- Power supply 24V
- Mini/micro USB cable
- Low-voltage 3-phase permanent magnet motor
- Personal Computer

Overview
========
The 3-phase permanent magnet synchronous motor (PMSM) control reference application
is  intended to provide the example for 3-phase PMSM motor control solutions.
The reference design uses a closed-loop field-oriented vector speed (FOC) control mechanism.

More information
================
Application web page with the board specific Application User's guide and other documentation: http://www.nxp.com/sdkmotorcontrol

Board settings
==============
Route the over-current fault signal by connection CMP0_IN1(P1_3, J2-17) with the MC1_CUR_DCB (P2_4, J2-9).
Follow chapter "Hardware setup" in Application User's guide for more information.

Running the demo
================
Follow chapter "User interface" in Application User's guide.

