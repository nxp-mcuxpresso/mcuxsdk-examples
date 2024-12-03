Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K32L3A6 board
- Personal Computer

Board settings
============
No special settings are required.
If download M0+ core project, need to let MCU boot from M0+ core, please follow below steps:
1. Download blhost.exe from www.nxp.com/kboot.
2. Connect J8 on baord to PC using USB cable.
3. After PC recognize the USB HID device, go to blhost.exe folder, open command line.
4. Run command “blhost.exe -u -- flash-erase-all-unsecure”, it will erase the flash on chip.
5. Run command “blhost.exe -u -- flash-program-once 0x84 4 ffffffbf”, set FOPT3 to boot from M0+ core.

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
