Hardware requirements
=====================
- Mini/micro USB cable
- IMX943-EVK board
- FRDM-LVPMSM-FA board
- Personal Computer
- An encoder with PHASE A/B signals.

Board settings
============
1. Set SW8 from [00000000] to [00011000] 
2. Insert the FRDM board into the MC2 arduino interface on the IMX943-EVK board
3. Set FRDM board
  - Set SW30 from [0000] to [0100]
  - Set SW90 from [0000] to [1000]
  - Set Jumper J61[2-3]
4. Connection between board and encoder
  - J60-1 -> 5V cable of encoder
  - J60-2 -> GND cable of encoder
  - J60-3 -> ENC_A cable of encoder
  - J60-4 -> ENC_B cable of encoder
  - J60-5 -> ENC_I cable of encoder
5. Connect 12V power to FRDM board
6. If you don't have FRDM-LVPMSM-FA board, you can connect it follows bellow:
  - J49-8 -> 3.3V cable of encoder
  - J49-14 -> GND cable of encoder
  - J49-3 -> ENC_A cable of encoder
  - J49-1 -> ENC_B cable of encoder
  - J48-3 -> ENC_I cable of encoder

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
