Toolchain supported
===================
- IAR Embedded Workbench (IDE version details are in the Release Notes)
- MCUXpresso IDE (IDE version details are in the Release Notes)
- GCC ARM Embedded (IDE version details are in the Release Notes)

Hardware requirements
=====================
- Type-C USB cable
- frdmmcxw70 board

Board settings
==============
No special board setting.

Prepare the Demo
================
1. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
2.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
3.  Download the program to the target board.
4.  Press the reset button on your board to begin running the demo.

Running the demo
================
This demo application can be used together with a Localization Reader demo.
### Running a Bluetooth Low Energy localization scenario using the shell interface
 - Enter the sb command on both terminals that are connected to the Localization Reader and Localization
User Device applications. The devices connect, perform pairing if not previously bonded, and trigger distance
measurement.
 - To trigger a new distance measurement with peer device id 0, enter “tdm 0” on the Localization User Device.

### Running the Bluetooth Low Energy Localization scenario using the button interface
 - Press SW2 on both boards to connect the devices. After the devices are connected, the link is encrypted and the Channel Sounding configuration is created, distance measurement is triggered.
 - Press SW3 on the loc_user_device board to trigger a new distance measurement.

Please consult the "Bluetooth Low Energy Localization Application Note" for more details.