# usb_pd_alt_mode_dp_dock



## Overview

This PD example is a simple demonstration based on the MCUXpresso SDK PD stack.
<br> The application use the shield dock board (om13790dock) to implement the DisplayPort alternate mode.
It recognize attached video source (like: "the shield host board (om13790host)" or "PC"), and drive the displayer to work.

## System Requirement

### Hardware requirements

- One Type-C shield dock board
- One 9V DC power supply
- Type-C Cable
- One hardware for a specific device, for example: one lpcxpresso54114 board
- Personal Computer or the shield host board (om13790host)


### Software requirements

- The project files are in: 
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_pd_alt_mode_dp_dock/<rtos>/<toolchain>.
> The <rtos> is Bare Metal or FreeRTOS OS.
- Terminal tool.


## Getting Started

### Hardware Settings

<br> For detailed instructions, see the appropriate board User's Guide.
> Please reference to the re-worked document for hardware settings.


### Prepare the example

1.  For MCUXpresso, please reference to the MCUXpresso SDK USB Type-C PD Stack User's Guide to make sure the SDK_DEBUGCONSOLE = 1 in project settings.
2.  Download the program to the target board.
3.  Power on shield dock board then power on development board.

## Run the example

1.  Download this program to the board.
2.  Connect the displayer to the MinDP port (J2).
3.  Connect one video source (like: "the shield host board (om13790host)" or "PC") to the Type-C port (J1).
    For example: if the video source is PC, connect the shield dock board to PC with one Type-C cable.
4.  The video source will recognize the shield dock board and the displayer works.

## Supported Boards
- LPCXpresso55S69
