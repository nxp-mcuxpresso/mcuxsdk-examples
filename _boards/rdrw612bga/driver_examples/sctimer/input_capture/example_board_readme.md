Hardware requirements
=====================
- Micro USB cable
- RD-RW61X-BGA board
- Personal Computer

Board settings
============
Connect HD2-5(SCT In 1) to 1kHz, dutycycle 50% PWM signal.

1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J7) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows example output of the SCTimer driver simple PWM demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SCTimer edge capture example

Once the input signal is received the capture values are printed
The input signal's pulse width is calculated from the capture values & printed

Capture2: 12301 Capture3: 17265 Pluse Width: 496 us
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
