Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMXRT700 board
- Personal Computer
- RaspberryPi Panel

Board settings
==============
To use Raspberry panel:
Connect the panel to J8. Then connect the panel's 5V pin to JP43-1, GND pin to JP43-2.

Prepare the Demo
================
The demo use MIPI Raspberry panel by default.
1.  Connect a USB cable between the PC host and the USB port on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
=====================
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
When the example runs, you can see the decoded jpeg image is displayed on the  
panel.
JPEG decode and EZHV linear2tile conversion demo start.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
