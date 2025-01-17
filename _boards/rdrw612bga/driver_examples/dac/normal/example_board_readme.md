Hardware requirements
=====================
- Micro USB cable
- RD-RW61X-BGA board
- Personal Computer

Board settings
============
Please remove R604.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J7) on the board.
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
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DAC Normal Driver Example!

Output = 0.18V + (1.42V * input_code / 1023)

Please input the value(Ranges from 0 to  1023) to be converted.
1000
Please input the value(Ranges from 0 to  1023) to be converted.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Use the oscilloscope or universal meter to probe HD2-16(GPIO_43), then the output voltage can be measured.
