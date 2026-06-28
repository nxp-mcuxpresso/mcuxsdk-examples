Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMXRT1152 board
- Personal Computer
- An encoder with PHASE A/B signals.

Board settings
============
1. For pin connection between board and encoder,
    J44.10 -> kXBARA1_InputIomuxXbarIn04 -> kXBARA1_OutputEnc1PhaseAInput -> ENC_PHA
    J44.12 -> kXBARA1_InputIomuxXbarIn05 -> kXBARA1_OutputEnc1PhaseBInput -> ENC_PHB
    J44.14 -> kXBARA1_InputIomuxXbarIn06 -> kXBARA1_OutputEnc1Index -> ENC_INDEX


Prepare the Demo
===============
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Connect the wires between encoder and the MCU board. See to the code for pin mux setting in function "BOARD_InitPins()".
3.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
