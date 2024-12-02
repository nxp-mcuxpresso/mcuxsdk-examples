Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA156 board
- Personal Computer
- An encoder with PHASE A/B signals.

Board settings
============
1. For pin connection between board and encoder,
      J4_5 -> kINPUTMUX_TrigIn10ToQdc0Phasea -> EQDC_PHA
      J4_3 -> kINPUTMUX_TrigIn9ToQdc0Phaseb -> EQDC_PHB
      J4_1 -> kINPUTMUX_TrigIn4ToQdc0Index -> EQDC_INDEX


Prepare the Demo
===============
1.  Connect a Type-C USB cable between the host PC and the MCU-Link USB port on the target board.
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
${ANCHOR}
