# usb_pd_source_charger



## Overview

The PD charger example is a simple demonstration based on the MCUXpresso SDK PD stack.
<br> The application simulate charger product.
<br> The demo only works as source and is external powered.

## System Requirement

### Hardware requirements

- One or two Type-C shield board
- One or two 9V DC power suppliers
- Type-C Cable
- One or two hardwares (Tower module/base board, and so on) for a specific device, for example: lpcxpresso54114 board
- Personal Computer


### Software requirements

- The project files are in: 
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_pd_source_charger/<rtos>/<toolchain>.
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

1.  Connect the OpenSDA USB port to the PC and open terminal.
2.  This charger provide power 5V/2.7A and 9V/1.5A.
3.  Connect the sink with Type-C cable to the board, The board will print the sink's request power information.
    <br> For example: Download usb_pd_charger_battery or usb_pd_sink_battery demo to another board and connect to the tested board.

## Supported Boards
- EVKB-IMXRT1050
- MIMXRT1060-EVKB
- MIMXRT1170-EVKB
- MIMXRT1060-EVKC
- EVK-MIMXRT1015
- EVK-MIMXRT1020
- MIMXRT1024-EVK
- MIMXRT1040-EVK
- EVK-MIMXRT1064
- MIMXRT1160-EVK
- MIMXRT1180-EVK
- EVK-MIMXRT595
- EVK-MIMXRT685
- FRDM-K22F
- FRDM-K32L2A4S
- FRDM-MCXN236
- FRDM-MCXN947
- LPCXpresso55S16
- LPCXpresso55S28
- LPCXpresso55S36
- LPCXpresso55S69
- MCX-N5XX-EVK
- MCX-N9XX-EVK
- MIMXRT685-AUD-EVK
- MIMXRT700-EVK
