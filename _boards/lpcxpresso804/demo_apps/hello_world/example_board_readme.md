# Hardware Requirements
- Mini/micro USB cable
- LPCXpresso804 board
- Personal Computer

# Board Settings
No special settings are required.

# Run the Demo
1. Connect a micro USB cable between the PC host and the CMSIS DAP port(CN2 on the board).

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
- Board will print "hello world." to the terminal, and if users input some characters,
  board will send them back to the terminal emulator immediately.
