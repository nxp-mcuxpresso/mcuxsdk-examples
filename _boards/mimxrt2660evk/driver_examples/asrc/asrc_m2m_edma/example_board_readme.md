Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer
- Headphone (OMTP standard)

Board settings
==============
Set jumper J60 to position 2-3 to connect the SAI signals to the on-board
WM8962 audio codec.

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Insert the headphones into the headphone jack on MIMXRT2660-EVK board (J85).
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, you can hear the tone and the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~
ASRC m2m edma example

Playback raw audio data

    sample rate : xxxx

    channel number: xxxx

    frequency: xxxx.


Playback converted audio data

    sample rate : xxxx

    channel number: xxxx

    frequency: xxxx.

ASRC m2m edma example finished
 ~~~~~~~~~~~~~~~~~~~
