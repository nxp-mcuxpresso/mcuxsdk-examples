Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
connections required:
- R328 1-3
- R330 1-3

Connect differential input voltage to J99 (input range: -320mV ~ +320mV).
note that a differential input of 50 mV will produce a stream of ones and zeros
that are high 89.06% of the time, not 100%.
So result can be verified based on equation: SINC result =(ADCin / 64mV) * (128^3 )

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Complete the wiring described in Board settings above.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SINC ADC Example.

Press any key to trigger conversion!

Adc Result: 1193

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
