Hardware requirements
=====================
- Type-C USB cable
- FRDM-KW43 Board
- Personal Computer

Board settings
==============
Connect J24-3 (PTA5 ADC0_A3) to a voltage source.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the FRDM board J28.
2. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC DMA3 Example
ADC Full Range: XXXX
Please press any key to trigger the conversion.
ADC conversion word: 0xXXXXXXXX
ADC conversion value: XXXX
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
