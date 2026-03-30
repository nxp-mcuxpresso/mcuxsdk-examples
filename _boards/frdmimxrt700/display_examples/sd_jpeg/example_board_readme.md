Hardware requirements
===================
- Micro USB cable
- FRDM-IMXRT700
- Personal Computer
- RaspberryPi Panel

Board settings
============
Make sure JP65 1-2 are connected, and insert the SD card which has the pictures unziped in root folder in slot J47.

To use Raspberry panel:
Connect the panel to J8. Then connect the panel's 5V pin to JP43-1, GND pin to JP43-2.
Make sure the R75, R76, R79, R80 are connected.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the MCU-LINK USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
If this example runs correctly, pictures are shown in the lcd panel one by one.
