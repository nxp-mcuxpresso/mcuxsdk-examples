# Connectivity test application

## 1 - Overview
========
After power on, the board will start flashing all LEDs and will print the application logo into a serial port terminal, signalling an idle state.
To start the application the [ENTER] key on the keyboard. Then follow the onscreen instructions to configure and run available tests.

This demo requires a serial port terminal application through a serial interface.

For more information please refer to `Generic FSK Link Layer Quick Start Guide pdf <../../../_static/wireless/genfsk/Generic_FSK_Link_Layer_Quick_Start_Guide.pdf>`_

## 2 - Toolchain supported
===================
- IAR embedded Workbench
- MCUXpresso IDE
- ARM GCC

## 3 - Hardware requirements
=====================
- Mini/micro USB cable
- Supported board: KW45B41Z-EVK, FRDM-MCXW71, KW47-EVK, MCX-W72-EVK
- Personal Computer a serial port terminal application installed.

## 4 - Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for the detected serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Press the reset button.
5.  Press [ENTER] in the console to display the main menu, or any other key to display the logo.
6.  Follow onscreen instructions to configure and run any available test.
7.  Repeat the steps above to prepare a second board for tests that require more than one.
