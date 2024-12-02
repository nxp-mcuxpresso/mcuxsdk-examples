# Functional description
This demo is to show how the various power mode can switch to each other.
There is one known issue that terminal may receive mess code after enter deep power down mode, it is caused by
the uart TX pin state changed by the MCU under deep power down mode. As the RM says, all the functional pins are
tri-stated in deep power down mode except for the WAKEUP pin. So the state change will be captured by
the virtual com port as mess code. Software has no workaround for the issue.
Note:
The power consumption data for Sleep mode can reach 0.4mA.
The power consumption data for Deep Sleep mode can reach 100uA.
The power consumption data for Power Down mode can reach 9uA. If want to reach 6uA, please disable LPOsc.
The power consumption data for Deep Power Down mode can reach 0.15uA.

# Hardware Requirements
- Mini/micro USB cable
- LPCXpresso804 board
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

	1. S1,wakeup key

	2. Wkt timer

	3. S3, reset key
/* after wakeup, terminal will output */
Wakeup.
