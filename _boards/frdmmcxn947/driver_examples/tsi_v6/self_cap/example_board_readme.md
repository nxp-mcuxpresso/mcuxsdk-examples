Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN947 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
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
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
TSI_V6 Self-Cap mode Example Start!
Calibrated counters for channel 0 is: 4328 
Calibrated counters for channel 1 is: 4310 
Calibrated counters for channel 2 is: 5095 
Calibrated counters for channel 3 is: 65535 
Calibrated counters for channel 4 is: 8748 
Calibrated counters for channel 5 is: 8512 
Calibrated counters for channel 6 is: 7375 
Calibrated counters for channel 7 is: 7464 
Calibrated counters for channel 8 is: 4935 
Calibrated counters for channel 9 is: 4872 
Calibrated counters for channel 10 is: 65535 
Calibrated counters for channel 11 is: 5266 
Calibrated counters for channel 12 is: 5419 
Calibrated counters for channel 13 is: 4494 
Calibrated counters for channel 14 is: 4822 
Calibrated counters for channel 15 is: 4932 
Calibrated counters for channel 16 is: 5070 
Calibrated counters for channel 17 is: 65535 
Calibrated counters for channel 18 is: 65535 
Calibrated counters for channel 19 is: 4056 
Calibrated counters for channel 20 is: 8420 
Calibrated counters for channel 21 is: 4048 
Calibrated counters for channel 22 is: 65535 
Calibrated counters for channel 23 is: 65535 
Calibrated counters for channel 24 is: 65535 

NOW, comes to the software trigger scan using polling method!
Channel 19 Normal mode counter is: 4056

NOW, comes to the software trigger scan using interrupt method!
Channel 19 Normal mode counter is: 4183

NOW, comes to the hardware trigger scan method!
After running, touch pad E1 each time, you will see LED toggles.
~~~~~~~~~~~~~~~~~~~~~
