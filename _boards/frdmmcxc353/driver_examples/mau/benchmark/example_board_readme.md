Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXC353 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
The log below shows the output of this demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MCUX SDK version: 2026.03.00
MAU benchmark example
Float SQRT(DSP)	: 1185 ms
Float SQRT(MAU)	: 163 ms
Float SIN(DSP)	: 3449 ms
Float SIN(MAU)	: 150 ms
Float COS(DSP)	: 3749 ms
Float COS(MAU)	: 146 ms
Float ATAN(DSP)	: 7424 ms
Float ATAN(MAU)	: 151 ms
Q31 SQRT(DSP)	: 2539 ms
Q31 SQRT(MAU)	: 146 ms
Q31 SIN(DSP)	: 680 ms
Q31 SIN(MAU)	: 150 ms
Q31 COS(DSP)	: 697 ms
Q31 COS(MAU)	: 150 ms
Q31 ATAN(DSP)	: 7770 ms
Q31 ATAN(MAU)	: 146 ms
Q15 SQRT(DSP)	: 885 ms
Q15 SQRT(MAU)	: 163 ms
Q15 SIN(DSP)	: 351 ms
Q15 SIN(MAU)	: 163 ms
Q15 COS(DSP)	: 368 ms
Q15 COS(MAU)	: 163 ms
Q15 ATAN(DSP)	: 2923 ms
Q15 ATAN(MAU)	: 163 ms
MAU benchmark example successed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
