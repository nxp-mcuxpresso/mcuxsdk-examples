Toolchain supported
===================
- IAR Embedded Workbench (IDE version details are in the Release Notes)
- MCUXpresso IDE (IDE version details are in the Release Notes)
- GCC ARM Embedded (IDE version details are in the Release Notes)

Hardware requirements
=====================
- Type-C USB cable
- MCX-W72-EVK Board
- Personal Computer

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
3.  Download the program to the target board.
4.  Press the reset button on your board to begin running the demo.
5.  Open a serial terminal application and use the following settings with the detected serial device:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

**Ensure that both the Loc Reader NCP (for the NBU core) and the Loc Reader Host (for the Application core) have been flashed to the board.**

Running the demo
================
The application is built to work with another supported platform running the loc_user_device

### Running a Bluetooth Low Energy localization scenario using the shell interface
 - Enter the sb command on both terminals that are connected to the Localization Reader and Localization
User Device applications. The devices connect, perform pairing if not previously bonded, and trigger distance
measurement.
 - To trigger a new distance measurement with peer device id 0, enter “tdm 0” on the Localization User Device.
