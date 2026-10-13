Hardware requirements
=====================
- USB Type-C cable
- FRDM-RW610FML board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the PC host and the MCU-Link USB port (J15) on the board.
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
The log below shows the output of the dac waveform generator demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 DAC Waveform Generator Example!

Please select the waveform to be generated:
         A -- Full Triangle wave
         B -- Half Triangle wave
         C -- Sine wave
         D -- Noise wave
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Use the oscilloscope to probe J4-4(GPIO_43), then the generated waveform can be observed.
