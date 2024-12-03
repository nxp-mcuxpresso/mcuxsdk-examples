Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA276 board
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
MAU benchmark example
Float SQRT(DSP) : 16 ms
Float SQRT(MAU) : 11 ms
Float SIN(DSP)  : 33 ms
Float SIN(MAU)  : 11 ms
Float COS(DSP)  : 35 ms
Float COS(MAU)  : 11 ms
Float ATAN(DSP) : 87 ms
Float ATAN(MAU) : 11 ms
Q31 SQRT(DSP)   : 62 ms
Q31 SQRT(MAU)   : 11 ms
Q31 SIN(DSP)    : 28 ms
Q31 SIN(MAU)    : 10 ms
Q31 COS(DSP)    : 30 ms
Q31 COS(MAU)    : 10 ms
Q31 ATAN(DSP)   : 335 ms
Q31 ATAN(MAU)   : 11 ms
Q15 SQRT(DSP)   : 50 ms
Q15 SQRT(MAU)   : 10 ms
Q15 SIN(DSP)    : 29 ms
Q15 SIN(MAU)    : 11 ms
Q15 COS(DSP)    : 30 ms
Q15 COS(MAU)    : 11 ms
Q15 ATAN(DSP)   : 193 ms
Q15 ATAN(MAU)   : 11 ms
MAU benchmark example successed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
