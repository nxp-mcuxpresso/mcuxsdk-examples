# Functional description
This demo is to show how the various power mode can switch to each other.

# Hardware Requirements
- Mini/micro USB cable
- LPCXpresso824MAX board
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
Power mode switch Demo for LPC8xx.

Selecct an option

	1. Sleep mode

	2. Deep Sleep mode

	3. Power Down mode

	4. Deep power down mode
/* after select power mode, terminal will output */
Select wakeup source

    1. Wkt timer

    2. SW1, wakeup key

    3. SW3, reset key
/* after wakeup, terminal will output */
Wakeup.
