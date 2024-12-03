Hardware requirements
===================
- Micro USB cable
- TWR-KM35Z75M board
- Personal Computer
- Elevator Tower

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Connect the analog signal source's output to the input of used-defined comparator's channel (defined as 
"DEMO_CMP_USER_CHANNEL" in source code.)
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Reset the SoC and run the project.

Running the demo
===============
When the demo runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
CMP polling Example
~~~~~~~~~~~~~~~~~~~~~

Then change CMP analog input, and watch the change of LED.
     - CMP0_IN1 (Jump A63 on Elevator Tower) connected to VCC = 3.3V (Jump B3 on Elevator Tower): LED ORANGE on
     - CMP0_IN1 (Jump A63 on Elevator Tower) connected to GND ( Jump B2 on Elevator Tower): LED ORANGE off
