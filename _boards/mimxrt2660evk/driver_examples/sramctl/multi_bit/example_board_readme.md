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
Multi-bit ECC errors are uncorrectable.

When the example runs successfully and the platform reports the event to software, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MCUX SDK version: <MCUXSDK_VERSION_FULL_STR>
SRAMCTL base address: 0x44080000
Init range: 0x2203FF00 - 0x2203FF0F

SRAMCTL multi-bit injection test
Target address : 0x2203FF00
Inject mask    : 0x00000003 (flip 2 data bits)
NOTE: multi-bit ECC is uncorrectable; some targets may fault/reset.
SRAMCTL status: 0xXXXXXXXX | SYND=0x<SS> EINFO=0x<EE> ECCNT=<N>
PASS: uncorrectable multi-bit error was detected (MLTERR=1)

SRAMCTL multi-bit example finished successfully.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
