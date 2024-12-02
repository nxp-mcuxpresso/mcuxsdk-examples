Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN236 board
- Personal Computer
- An encoder with PHASE A/B signals.

Board settings
============
1. For pin connection between board and encoder,
      J1.3(P2_0)  -> kINPUTMUX_TrigIn5ToQdc0Index  -> QDC_INDEX
      J3.3(P4_21) -> kINPUTMUX_TrigIn9ToQdc0Phasea -> QDC_PHA
      J3.1(P4_13) -> kINPUTMUX_TrigIn8ToQdc0Phaseb -> QDC_PHB

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the MCU-Link USB port on the target board.
2.  Connect the wires between encoder and the MCU board. See to the code for pin mux setting in function "BOARD_InitPins()".
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
${ANCHOR}

