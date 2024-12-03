Hardware requirements
=====================
- Mini/micro USB cable
- MCX-N9XX-EVK board
- Personal Computer

Board settings
============

Prepare the Demo
===============
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
===============
When the demo runs successfully, the log would be seen on the MCU-Link terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~
PUFv3 driver example
1510000:TestSelftest:PASS
1510001:TestGenerateRandomData:PASS
1510002:TestStart:PASS
1510003:TestDeriveDeviceKey:PASS
1510004:TestRandomWrapUnwrap:PASS
1510005:TestUserKeyWrapUnwrap:PASS
1510006:TestStop:PASS
1510007:TestZeroize:PASS
-----------------------
8 Tests 0 Failures 0 Ignored
OK
~~~~~~~~~~~~~~~~~~~~~~~~~~~

