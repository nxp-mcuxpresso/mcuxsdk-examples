Hardware requirements
=====================
- Micro USB cable
- LPCXpresso55S06 board
- Personal Computer

# Board settings

```
Output signal		Board location
SCT0_OUT2    		J12-12
SCT0_OUT6    		J12-17
```

# Prepare the Demo

1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J1) on the target board.
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
- Probe oscilloscope at J12_12(5HZ) and J12_17(2.5HZ) to see output signal.