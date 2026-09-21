Hardware requirements
=====================
- USB-C cable
- FRDM-MCXC353 board
- Personal Computer

Board settings
==============
FTM output: P2_3(LED_GREEN)

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the MCU-LINK USB port on the target board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the FTM combine PWM example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FTM example to output center-aligned PWM signal
You will see a change in LED brightness if an LED is connected to the FTM output pin.
If no LED is available, probe the signal on oscilloscope.
Else, the test is all done.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
