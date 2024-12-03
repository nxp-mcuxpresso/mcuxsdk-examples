Hardware requirements
===================
- Mini/Micro USB cable
- FRDM-K32L2B board
- Personal Computer

Board settings
============
No special is needed.

Prepare the Demo
===============
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Connect external signal source to user-defined channel, defined by "DEMO_ADC16_CHANNEL" in source code.
    Shorten jumper J19 to use external reference voltage pair pins.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

ADC16 CONTINUOUS DMA DEMO
ADC16_DoAutoCalibration() Done.
ADC Full Range: XXXX
Press any key to get user channel's ADC value ...
ADC value: 32878
ADC value: 34463
ADC value: 36226
ADC value: 32819
ADC value: 37477

