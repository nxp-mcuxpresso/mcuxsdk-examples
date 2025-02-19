Hardware requirements
=====================
- USB-C cable
- FRDM-MCXE247 board
- Personal Computer
- USB to COM converter

Board settings
==============
PTC9(J5-4) connect to RxD of the USB to COM converter.
PTC8(J5-3) connect to TxD of the USB to COM converter.
GND(J5-8) connect to GND of the USB to COM converter.

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
