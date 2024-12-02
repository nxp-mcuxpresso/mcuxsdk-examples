Hardware requirements
=====================
- Micro USB cable
- FRDM-KE17Z512 board
- Personal Computer
- USB to Com converter

Board settings
==============
PTA2(J1-2) connect to TxD of the USB to Com converter.
PTA3(J1-4) connect to RxD of the USB to Com converter.
GND(J3-14) connect to GND of the USB to Com converter.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the MCU-LINK USB port on the target board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 7 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal(from USB To Serial Adapter):


~~~~~~~~~~~~~~~~~~~~~
Lpuart polling example with seven data bits
Board will send back received characters
~~~~~~~~~~~~~~~~~~~~~
