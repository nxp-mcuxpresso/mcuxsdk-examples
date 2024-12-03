Hardware requirements
=====================
- Micro USB cable
- MCX-N9XX-EVK board
- Personal Computer

# Board settings

```
Output signal		Board location
SCT0_OUT4    		J3-3 (P1_22)
SCT0_OUT5    		J1-8 (P1_23)
```

# Prepare the Demo

1.  Connect a micro USB cable between the host PC and the MCU-Link USB port (J5) on the target board.
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
The log below shows example output in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SCTimer example to use it in 16-bit mode
The example shows both 16-bit counters running and toggling an output periodically
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You'll see PWM signals on J3-3 and J1-8 using an oscilloscope