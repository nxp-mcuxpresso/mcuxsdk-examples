Hardware requirements
===================
- Mini/micro USB cable
- LPCXpresso55S16 board
- Personal Computer

Board settings
============
(pmux)J12-19(PIO0_18) connect to voltage source1.
(nmux)J12-3(PIO1_14) connect to voltage source2.
The jumper setting:
    Default jumpers configuration does not work,  you will need to add JP20 and JP21 (JP22 optional for ADC use)

Prepare the Demo
===============
1.  Connect a macro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

CMP interrupt driver example.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Then change CMP analog input, and the LED GREEN will show the output of the comparator.
    If the voltage of pmux channel is higher than the voltage of nmux channel, the GREEN LED on the board will be turned on.
    If the voltage of pmux channel is lower than the voltage of nmux channel, the GREEN LED on the board will be turned off.
