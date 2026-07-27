Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer
- An encoder with PHASE A/B/INDEX signals.

Board settings
==============
Connect encoder signals via GPIO TrigOut → XBAR2 → XBAR0 → EQDC0:
- Phase A: J94-13 (PIO3_0,  GPIO1_GPIO0,  TrigOut0) → EQDC0_PHASE_A_IN
- Phase B: J94-14 (PIO3_1,  GPIO1_GPIO1,  TrigOut1) → EQDC0_PHASE_B_IN
- Index:   J33-4  (PIO2_8,  GPIO0_GPIO8,  TrigOut0) → EQDC0_INDEX_IN

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Connect the encoder signals to J94-13/14 (Phase A/B) and J33-4 (Index) as described in Board settings.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
