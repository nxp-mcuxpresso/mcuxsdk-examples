Hardware requirements
=====================
- FRDMMCXE32B board
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
Follow chapter "Hardware setup" in Application User's guide.

In this example, the LCU is used as a PWM output, therefore some additional wiring is required.
Please connect the FRDM-MC-LVPMSM to the FRDM-MCXE32B, and then connect the LCU outputs on J8 to the Freedom board using wires.

FRDM-MC-LVPMSM      FRDM-MCXE32B
J3-15 (PWM_AT) with J8-9  (PTB1)
J3-13 (PWM_AB) with J8-11 (PTB0)
J3-11 (PWM_BT) with J8-6  (PTC9)
J3-9  (PWM_BB) with J8-8  (PTC8)
J3-7  (PWM_CT) with J8-10 (PTC11)
J3-5  (PWM_CB) with J8-12 (PTC10)

Running the demo
================
Follow chapter "User interface" in Application User's guide.

