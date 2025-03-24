Hardware requirements
=====================
- Mini/micro USB cable
- MCXW23-EVK Board
- Personal Computer

Board settings
==============

```
Output signal		Board location
SCT0_OUT4    		J20-5
SCT0_OUT5   		J20-6
```

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the EVK board J33.
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
The log below shows example output of the SCTimer driver simple PWM demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SCTimer example to output 2 center-aligned PWM signals

Probe the signal using an oscilloscope
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Probe an oscilloscope to the output pins described in board settings, you will see two PWM signals.
 