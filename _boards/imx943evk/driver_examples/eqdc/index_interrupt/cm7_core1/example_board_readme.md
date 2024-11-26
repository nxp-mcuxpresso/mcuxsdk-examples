Hardware requirements
=====================
- Mini/micro USB cable
- IMX943-EVK board
- Personal Computer
- An encoder with PHASE A/B signals.

Board settings
============
1. For pin connection between board and encoder,
      J47-10(pad name: GPIO_IO9) -> kXBAR1_InputRESERVED44 -> kXBAR1_OutputEnc1Phasea -> EQDC_PHA
      J47-8(pad name: GPIO_IO10) -> kXBAR1_InputRESERVED45 -> kXBAR1_OutputEnc1Phaseb -> EQDC_PHB
      J47-12(pad name: GPIO_IO11) -> kXBAR1_InputRESERVED46 -> kXBAR1_OutputEnc1Index -> EQDC_INDEX


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

Running the demo
================
${ANCHOR}
