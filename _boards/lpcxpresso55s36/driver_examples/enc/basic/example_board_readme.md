Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso55S36 board
- Personal Computer
- An encoder with PHASE A/B signals.

Board settings
============
1. For pin connection between board and encoder,
      J10.3 -> kINPUTMUX_ExtTrigIn3ToEnc0Phasea -> ENC_PHA
      J10.1 -> kINPUTMUX_ExtTrigIn2ToEnc0Phaseb -> ENC_PHB
      J12.3 -> kINPUTMUX_ExtTrigIn8ToEnc0Index  -> ENC_INDEX

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
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

