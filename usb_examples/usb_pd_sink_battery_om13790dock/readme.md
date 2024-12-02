# usb_pd_sink_battery



## Overview

The PD sink battery example is a simple demonstration based on the MCUXpresso SDK PD stack.
<br> The application simulate battery product (for example: laptop), it prints the battery percent continually.
<br> The demo works as sink and get power from partner port.

## System Requirement

### Hardware requirements

- One or two Type-C shield board
- One or two 9V DC power suppliers
- Type-C Cable
- One or two hardwares (Tower module/base board, and so on) for a specific device, for example: lpcxpresso54114 board
- Personal Computer


### Software requirements

- The project files are in: 
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_pd_sink_battery/<rtos>/<toolchain>.
> The <rtos> is Bare Metal or FreeRTOS OS.
- Terminal tool.


## Getting Started

### Hardware Settings

<br> For detailed instructions, see the appropriate board User's Guide.
> Set the hardware jumpers (Tower system/base module) to default settings.


### Prepare the example

1.  Download the program to the target board.
2.  Power on Type-C shield board then power on development board.

## Run the example

1.  Connect the board to one charger or another shield board + development board (download usb_pd_charger_battery, usb_pd_source_charger demo) with Type-C cable.
2.  Connect the OpenSDA USB port to the PC and open terminal.
3.  When don't connect to charger. The battery percent reduce and print in debug console.
    When battery percent reduce to zero, the demo doesn't work as ture product and power off, it just keep print battery percent.
4.  When connect to charger, running as sink.
    The demo will request the highest voltage that self and the partner charger support. The demo will simulate that the battery is charging and print the increased battery percent.

## Supported Boards
- LPCXpresso55S69
