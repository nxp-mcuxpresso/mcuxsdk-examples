Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KE02Z40M board
- Personal Computer

Board settings
============
ADC0_SE0 is ADC input channel and J2-2 is ADC input pin.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
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
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADC interrupt Example.
ADC Full Range: XXXX
Press any key to get user channel's ADC value.
ADC Value: 38
ADC Interrupt Counter: 1
ADC Value: 16
ADC Interrupt Counter: 2
ADC Value: 14
ADC Interrupt Counter: 3
ADC Value: 18
ADC Interrupt Counter: 4
ADC Value: 15
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The user can sample voltage by J2-2 pin.