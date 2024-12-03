Hardware requirements
===================
- Micro USB cable
- LPCXpresso55S28 board
- Adafruit TFT LCD shield w/Cap Touch V2.3
- Personal Computer

Board settings
============
There are different versions of Adafruit 2.8" TFT LCD shields. The shields marked
v2.0 works directly with this project. For the other shields, please solder
the center pin of IOREF pads to the 3.3V pad, and solder the pads 11, 12, and 13.
See the link for details:
https://community.nxp.com/t5/MCUXpresso-Community-Articles/Modifying-the-latest-Adafruit-2-8-quot-LCD-for-SDK-graphics/ba-p/1131104

Attach the LCD shield to the LPC board.

Prepare the Demo
===============
1.  Power the board using a micro USB cable connected to P5 USB port on the board, attach debugger to P7 connector
2.  Build the project.
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
===============
Known issue: The MCU is not able to generate hardware reset of the LCD due to hardware limitation of the boards interconnection.
Because of this the LCD may not get initialized (stays blank) following a cold start (power on) of the board even if the demo was
correctly programmed to the FLASH memory. In such a case simply press the reset button to restart the demo.
If user use Adafruit TFT LCD V2.8, please refer to this link: https://community.nxp.com/community/mcuxpresso/blog/2020/01/10/modifying-the-latest-adafruit-28-lcd-for-sdk-graphics-examples
