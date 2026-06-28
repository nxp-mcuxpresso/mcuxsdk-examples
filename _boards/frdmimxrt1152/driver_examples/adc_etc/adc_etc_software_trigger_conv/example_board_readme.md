Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMXRT1152 board
- Personal Computer

Board settings
============
ADC1_IN0 is ADC input. Please sample voltage by J47-8 pin.

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
ADC_ETC_Software_Trigger_Conv Example Start!
ADC Full Range: 4096
Press any key to get user channel's ADC value.
ADC conversion value is 3056
Press any key to get user channel's ADC value.
ADC conversion value is 1909
Press any key to get user channel's ADC value.
ADC conversion value is 2062
Press any key to get user channel's ADC value.
ADC conversion value is 1979
Press any key to get user channel's ADC value.
ADC conversion value is 1605
Press any key to get user channel's ADC value.
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


