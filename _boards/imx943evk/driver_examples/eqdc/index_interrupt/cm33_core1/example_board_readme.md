Hardware requirements
=====================
- Mini/micro USB cable
- IMX943-EVK board
- Personal Computer
- An encoder with PHASE A/B signals.

Board settings
============
1. For pin connection between board and encoder,
      J47-10(name: M2_SPI3_MISO, pad name: GPIO_IO9) -> EQDC_PHA
      J47-8(name: M2_SPI3_MOSI, pad name: GPIO_IO10) -> EQDC_PHB
      J47-12(name: M2_SPI3_SCLK, pad name: GPIO_IO11) -> EQDC_INDEX

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
