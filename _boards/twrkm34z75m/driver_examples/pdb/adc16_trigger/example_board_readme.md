Hardware requirements
=====================
- Mini USB cable
- TWR-KM34Z75M board
- Personal Computer

Board settings
==============
Connect J21 1-2 to use potentiometer

Prepare the Demo
================
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
Roll the potentiometer to change the ADC value.
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~
PDB ADC16 Pre-Trigger Example.

Type any key into terminal to trigger the PDB and then trigger the ADC's conversion ...

PDB Interrupt Counter: 1
ADC Conversion Interrupt Counter: 1
ADC Conversion Value: 4095
................
................
~~~~~~~~~~~~~~~~~~~~~~~~
