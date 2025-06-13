Hardware requirements
=====================
- Mini/micro USB cable
- imx943evk board
- Personal Computer

Board settings
==============
Connect J43-12 to oscilloscope to capture the trigger signal.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the USB DBG port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Program flash.bin to boot source and start the board.
4. Link up the swp2 phy.

Running the demo
===============
When the demo runs successfully, can get timer FIPER signal on OSC.

