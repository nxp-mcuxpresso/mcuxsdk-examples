Hardware requirements
=====================
- Micro USB cable
- LPCXpresso55S28 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port P6) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Power Manager Demo for LPC device.
The "user key" is: SW3
Select an option
	1. Sleep mode
	2. Deep Sleep mode
	3. power down mode
	4. Deep power down mode
/* Type in '1' into UART terminal */
Entering Sleep [Press the user key to wakeup] ...
/* Press the user key on board */
Pin event occurs
Wakeup.
Select an option
	1. Sleep mode
	2. Deep Sleep mode
	3. power down mode
	4. Deep power down mode
/* Type in '2' into UART terminal */
Entering Deep Sleep [Press the user key to wakeup] ...
/* Press the user key on board */
Pin event occurs
Wakeup.
Select an option
	1. Sleep mode
	2. Deep Sleep mode
	3. power down mode
	4. Deep power down mode
/* Type in '3' into UART terminal */
Entering Deep Powerdown [Reset to wakeup] ...
Press any key to confirm to enter the deep sleep mode and wakeup the device by press s1 key on board.
/* Type in any key into UART terminal */

/* Press the s1 key on board. */
Wakeup.
Select an option
	1. Sleep mode
	2. Deep Sleep mode
	3. power down mode
	4. Deep power down mode
/* Type in '4' into UART terminal */
Entering Deep Powerdown [Reset to wakeup] ...
Press any key to confirm to enter the deep power down mode and wakeup the device by reset.
/* Type in any key into UART terminal */
Please input the number of second to wait for waking up
The second must be positive value
/* Type in 3 into UART terminal */
System will wakeup at 3s later
Power Manager Demo for LPC device.
The "user key" is: SW3
Select an option
	1. Sleep mode
	2. Deep Sleep mode
	3. power down mode
	4. Deep power down mode
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
