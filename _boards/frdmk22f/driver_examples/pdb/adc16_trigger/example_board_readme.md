Hardware requirements
===================
- Mini/Micro USB cable
- FRDM-K22F board
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
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

PDB ADC16 Pre-Trigger Example.
ADC Full Range: XXXX
Type any key into terminal to trigger the PDB and then trigger the ADC's conversion ...

PDB Interrupt Counter: 1
ADC Conversion Interrupt Counter: 1

Type any key into terminal to trigger the PDB and then trigger the ADC's conversion ...

PDB Interrupt Counter: 2
ADC Conversion Interrupt Counter: 2
