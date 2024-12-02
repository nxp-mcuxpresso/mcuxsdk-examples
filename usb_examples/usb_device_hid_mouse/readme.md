# dev_hid_mouse




## Overview

The USB HID mouse application is a simple demonstration program based on the MCUXpresso SDK.
It is enumerated as a mouse. Users can see the mouse arrow moving on the PC screen according in a rectangular fashion.
## System Requirements

### Hardware requirements

- Mini/micro USB cable
- USB A to micro AB cable
- Hardware (Tower module/base board, and so on) for a specific device
- Personal Computer (PC)


### Software requirements

- The project files are in:
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_device_hid_mouse/<rtos>/<toolchain>.
<br> For lite version, the project files are in:
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_device_hid_mouse_lite/<rtos>/<toolchain>.
> The <rtos> is Bare Metal or FreeRTOS OS.


## Getting Started

### Hardware Settings

> Set the hardware jumpers (Tower system/base module) to default settings.


### Prepare the example

1.  Download the program to the target board.
2.  Connect the target board to the external power source (the example is self-powered).
3.  Power off the target board. Then power on again.
4.  Connect a USB cable between the PC and the USB device port of the board.

> For detailed instructions, see the appropriate board User's Guide.

## Run the example

1.  Plug-in the device, which is running HID mouse example, into the PC. A HID-compliant mouse is enumerated in the Device Manager.
2.  The mouse arrow is moving on PC screen in the rectangular rotation.

*/

## Supported Boards
- FRDM-K22F
- [FRDM-K32L2A4S](../../_boards/frdmk32l2a4s/usb_examples/usb_device_hid_mouse/example_board_readme.md)
- FRDM-K32L3A6
- FRDM-MCXA153
- FRDM-MCXA156
- FRDM-MCXA276
- FRDM-MCXN236
- [FRDM-MCXN947](../../_boards/frdmmcxn947/usb_examples/usb_device_hid_mouse/example_board_readme.md)
- LPCXpresso51U68
- LPCXpresso54628
- LPCXpresso54S018
- LPCXpresso54S018M
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/usb_examples/usb_device_hid_mouse/example_board_readme.md)
- LPCXpresso55S28
- LPCXpresso55S36
- LPCXpresso55S69
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/usb_examples/usb_device_hid_mouse/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/usb_examples/usb_device_hid_mouse/example_board_readme.md)
