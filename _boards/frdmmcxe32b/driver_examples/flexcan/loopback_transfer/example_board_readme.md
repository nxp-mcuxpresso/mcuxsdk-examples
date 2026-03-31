Hardware requirements
=====================
- Micro USB cable
- FRDM-MCXE32B board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a type-c USB cable between the host PC and the MCU-Link USB port (J13) on the target board. 
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
Consider special errata 005829 feature, the 1st valid MB should be used as reserved one.
The TX MB number will change from 8 to 9, while RX MB number will change from 9 to 10
When the example runs successfully, following information can be seen on the OpenSDA terminal:

~~~~~~~~~~~~~~~~~~~~~

==FlexCAN loopback example -- Start.==


Send message from MB0 to MB1

tx word0 = 0x11223344

tx word1 = 0x55667788


Receved message from MB1

rx word0 = 0x11223344

rx word1 = 0x55667788


==FlexCAN loopback example -- Finish.==
~~~~~~~~~~~~~~~~~~~~~
