Hardware requirements
=====================
- Micro USB cable
- MCX-N5XX-EVK board
- NXP FRDM-TOUCH board (expand the NXP Touch library features enabling to change LED’s hue or blinking durring movement and demonstrating mutual sensing keypad) 
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Download the program to the target board.
3.  Download a FreeMASTER tool (from: http://www.nxp.com/freemaster, version 3.2) and install the FreeMASTER
4.  Open the FreeMASTER project NXPTouch.pmpx file from: MCUXpresso SDK\middleware\touch\freemaster\ and choose Project -> Options... from FreeMASTER menu.  
5.  In Comm tab set the COM_ALL option or port created by the MCU board.
6.  Set the Speed to 19200 baud rate.
7.  Continue with help of the NXP Touch Library Reference Manual (Chapter 8 Examples or Chapter 5 Touch GUI Tool).

Running the demo
================
The control page in FreeMASTER should show changing values from TSI measurement. The Led on MCX-N5XX-EVK board 
should be shining after the touch event or should be blinking when finger is moving on the slider control. 
The LED color demonstrates the slider position and is changing from red through yellow, green and purple to blue color.
In case the red color shine without touching demo, debug the main function of application, code comments should explain wrong setting and find the solution. 
More information can be found in the NXP Touch Library Reference Manual (Chapter 8 Examples).
