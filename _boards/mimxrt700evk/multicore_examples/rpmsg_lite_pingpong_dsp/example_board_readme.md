Hardware requirements
=====================

- Mini/micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Board settings
==============

Unshort jumper JP27 to enable also Serial Console from Secondary Core.

Prepare the Demo
================

1. Connect the PC host and the board
2. Open a two serial terminals with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================

The log below shows the output of the RPMsg-Lite pingpong demo in the terminal window on primary core:

```
RPMsg demo starts Primary core.
Copy CORE1 image to address: 0x20600000, size: 102860
RPMsg demo starts DSP core 'rt700_hifi4_RI23_11_nlib'.

RPMsg demo starts
Sending data to Secondary core...
Primary core sending  a cm_msg. Message: Size=4, DATA = 0
Primary core received a cm_msg. Message: Size=4, DATA = 1
...
...
Primary core sending  a cm_msg. Message: Size=4, DATA = 98
Primary core received a cm_msg. Message: Size=4, DATA = 99
Sending data to HIFI4 core...
Primary core sending  a dsp_msg. Message: Size=4, DATA = 100
Primary core received a dsp_msg. Message: Size=4, DATA = 101
...
...
Primary core sending  a dsp_msg. Message: Size=4, DATA = 198
Primary core received a dsp_msg. Message: Size=4, DATA = 199

RPMsg demo ends
```

The log below shows the output of the RPMsg-Lite pingpong demo in the terminal window on secondary core:

```
RPMsg demo starts Secondary core.
RPMsg demo starts DSP core 'rt700_hifi1_RI23_11_nlib'.
Sending data to HIFI1 core...
Secondary core sending  a dsp_msg. Message: Size=4, DATA = 200
Secondary core received a dsp_msg. Message: Size=4, DATA = 201
...
...
Secondary core sending  a dsp_msg. Message: Size=4, DATA = 298
Secondary core received a dsp_msg. Message: Size=4, DATA = 299

RPMsg demo ends

```
