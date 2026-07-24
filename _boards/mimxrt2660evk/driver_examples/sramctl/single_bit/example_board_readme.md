Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the MCU-Link USB port on the target board.
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
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MCUX SDK version: <MCUXSDK_VERSION_FULL_STR>
SRAMCTL base address: 0x44080000
Init range: 0x2203FF00 - 0x2203FF0F

SRAMCTL single-bit injection test
Target address : 0x2203FF00
Inject mask    : 0x00000001 (flip 1 data bit)
Read value     : 0xA5A55A5A (corrected by ECC)
SRAMCTL status: 0xXXXXXXXX | SYND=0x<SS> EINFO=0x<EE> ECCNT=<N>
PASS: correctable single-bit error was detected and corrected (ECCNT=<N>)

SRAMCTL single-bit example finished successfully.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
