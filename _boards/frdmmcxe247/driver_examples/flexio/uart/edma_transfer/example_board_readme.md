Hardware requirements
=====================
- USB-C cable
- FRDM-MCXE247 board
- USB to UART converter
- Personal Computer

Board settings
==============
This example requires connecting the FLEXIO pins with the USB to UART pins
The connection should be set as following:
    - PTD2, FLEXIO_UART_RX_PIN, J2-8
    - PTE15, FLEXIO_UART_TX_PIN, J2-4
    - GND, J2-14

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the MCU-LINK USB port on the target board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal(from USB To Serial Adapter) as below:

~~~~~~~~~~~~~~~~~~~~~
Flexio uart edma example
Board receives 8 characters then sends them out
Now please input:
~~~~~~~~~~~~~~~~~~~~~
