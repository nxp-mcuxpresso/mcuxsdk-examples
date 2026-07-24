Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer
- An encoder with PHASE A/B signals.

Board settings
==============
Connect encoder signals via XBAR1 INOUT → XBAR0 → EQDC0:
- Phase A: J94-13 (PIO3_0, XBAR1_INOUT00) → EQDC0_PHASE_A_IN
- Phase B: J94-14 (PIO3_1, XBAR1_INOUT01) → EQDC0_PHASE_B_IN
- Index:   J94-15 (PIO3_2, XBAR1_INOUT02) → EQDC0_INDEX_IN

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Connect the encoder signals to J94-13/14/15 as described in Board settings.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
