Hardware requirements
=====================
- Micro USB cable
- FRDM-RW612 board
- Personal Computer

Board settings
==============
No special settings

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J10) on the board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADC Software Trigger Example!
Resolution: 16 bit.
Input Mode: Single Ended.
Input Range: 0V to 1.2V.

Calibration Success!
Please press any key to trigger conversion.


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Connect the analog signal to J7-3(GPIO_46)
Note: Analog input voltage minimum needs to be higher than 0.2v.
