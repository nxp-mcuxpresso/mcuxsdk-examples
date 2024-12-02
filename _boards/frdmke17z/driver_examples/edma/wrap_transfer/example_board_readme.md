Hardware requirements
=====================
- Micro USB cable
- FRDM-KE17Z board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
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
EDMA wrap transfer example.



edma modulo wrap start

0	0	0	0	0	0	0	0	

EDMA modulo wrap finished.

1	2	3	4	1	2	3	4	

edma major loop wrap start

0	0	0	0	0	0	0	0	

edma major loop wrap finished

1	2	3	4	5	6	7	8	
~~~~~~~~~~~~~~~~~~~~~
