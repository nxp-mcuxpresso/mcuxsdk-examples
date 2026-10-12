Hardware requirements
=====================
- Type-C USB cable
- IMXRTL345MB board
- Personal Computer

# Board settings

```
Output signal		Board location
SCT0_OUT0    		TBD (TBD)
SCT0_OUT4    		TBD (TBD)
```

# Prepare the Demo

1.  Connect a Type-C USB cable between the host PC and the MCU-Link USB port on the target board.
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

You'll see center-aligned PWM signals on TBD and TBD using an oscilloscope. 