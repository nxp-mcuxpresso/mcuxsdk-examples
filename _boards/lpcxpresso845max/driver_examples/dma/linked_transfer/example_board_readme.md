# Functional description
This example shows the usage of more than one descriptors to define a complicated transfer.

# Hardware Requirements
- Mini/micro USB cable
- LPCXpresso845MAX board
- Personal Computer

# Board Settings
No special settings are required.

# Run the Demo
1. Connect a micro USB cable between the PC host and the CMSIS DAP port(J4 on the board).

2. Open a serial terminal with the following settings:
   - 9600 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control

3. Choose an IDE, building the project and download the program to the target board.
   More information about how to compile and program the project can refer to the 

   [Getting Started with MCUXpresso SDK](../../../../docs/Getting Started with MCUXpresso SDK.pdf).

4. Launch the debugger in your IDE to begin running the demo.

# Expected Result
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~
DMA linked transfer example begin.

Destination Buffer:

0   0   0   0   0   0   0   0

DMA linked transfer example finish.

Destination Buffer:

1   2   3   4   11  22  33  44
~~~~~~~~~~~~~~~~~~~~~
