Hardware requirements
=====================
- Micro USB cable
- FRDM-KE17Z board
- FRDM-TOUCH board
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
TSI_V5 Mutual-Cap mode Example Start!

Please do not touch pad KEY3 when in calibration process!
Calibrated counters for mutual-cap pad KEY3 is: 2600 

NOW, comes to the software trigger scan using polling method!
Mutual-cap pad KEY3 Normal mode counter is: 2600 

NOW, comes to the software trigger scan using interrupt method!
Mutual-cap pad KEY3 Normal mode counter is: 2600 

NOW, comes to the hardware trigger scan method!
After running, touch pad KEY3 each time, you will see LED toggles
~~~~~~~~~~~~~~~~~~~~~
