Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA276 Board
- Personal Computer

Board settings
==============
Connect the USB to TTL converter to FRDM-MCXA276 board.
- TX of USB2COM to J5-3(P4_2)
- RX of USB2COM to J5-4(P4_5)
- GND of the USB2COM to J5-8(GND)

DAC output pin: J1-4(P2_2)

Prepare the Demo
================
1. Connect the type-C USB cable between the PC host and the USB ports on the board.
2. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.
5. A multimeter may be used to measure the DAC output voltage.

Running the demo
================
When the demo runs successfully, the log would be seen on the MCU-Link terminal like:

DAC buffer interrupt Example.
Press any key to trigger the DAC...
DAC next output: 200
DAC next output: 300
DAC next output: 400
DAC next output: 500
DAC next output: 600
DAC next output: 700
DAC next output: 800
DAC next output: 900

Then user can measure the DAC output pin to check responding voltage.

