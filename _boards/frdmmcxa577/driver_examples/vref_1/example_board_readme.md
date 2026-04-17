Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA577 board
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
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MCUX SDK version: 2026.06.00-pvw1

VREF example
ADC Full Range: 65536
Default (Factory) trim value is :0

Use trim value: 1
ADC conversion result: 24920
Actual voltage on VREF_OUT: 1.240V

Use trim value: 2
ADC conversion result: 29863
Actual voltage on VREF_OUT: 1.485V

Use trim value: 3
ADC conversion result: 35963
Actual voltage on VREF_OUT: 1.789V

Use trim value: 4
ADC conversion result: 40938
Actual voltage on VREF_OUT: 2.036V

Use trim value: 5
ADC conversion result: 49924
Actual voltage on VREF_OUT: 2.483V

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
