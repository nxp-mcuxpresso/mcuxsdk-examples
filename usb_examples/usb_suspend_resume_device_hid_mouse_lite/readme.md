# dev_suspend_resume_hid_mouse_lite_bm




## Overview

The Suspend/Resume USB HID mouse application is a simple demonstration program based on the MCUXpresso SDK.
It is enumerated as a mouse. Users can see the mouse arrow moving on the PC screen according in a rectangular fashion.
When host releases the USB Bus, the device will enter into low power mode. The device can be waked up when resume signal is detected on the bus.
Or the device can remote wake-up the host by delivering the resume signal when the remote wake-up is enabled by the host.

## System Requirements

### Hardware requirements

- Mini/micro USB cable
- USB A to micro AB cable
- Hardware (Tower module/base board, and so on) for a specific device
- Personal Computer (PC)


### Software requirements

- The project files are in:
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_suspend_resume_device_hid_mouse/<rtos>/<toolchain>.
<br> For lite version, the project files are in:
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_suspend_resume_device_hid_mouse_lite/<rtos>/
<toolchain>.
> The <rtos> is Bare Metal or FreeRTOS OS.


## Getting Started

### Hardware Settings

> Set the hardware jumpers (Tower system/base module) to default settings.


### Prepare the example

> For detailed instructions, see the appropriate board User's Guide.

## Run the example

1.  Plug-in the device, which is running Suspend/Resume HID mouse example, into the PC. A HID-compliant mouse is enumerated in the Device Manager.
2.  The mouse arrow is moving on PC screen in the rectangular rotation.
3.  Make sure your PC enable the USB remote wake support in BIOS.Press the sleep button to make your PC go to sleep and the USB bus becomes idle. Then the device enters into low power mode.
4.  If the remote wake-up feature is enabled by the host, the device is waked up and delivers the resume signal when the wakeup switch is pressed.
    The name of wakeup switch is showed in debug console when the remote wake-up feature is enabled.
    For example, if the name of wakeup switch is "SW3", the debug console output is as following. 
    ```
        USB device remote wakeup state: 1
        USB device start suspend
        USB device suspended.
        Please Press wakeup switch(SW3) to remote wakeup the host.
        Remote wakeup the host.
        USB device start resume
        USB device resumed.
    ```
    <br> Or the device is waked up when the resume signal is detected on the bus.
    ```
        USB device remote wakeup state: 0
        USB device start suspend
        USB device suspended.
        USB device start resume
        USB device resumed.
    ```



*/

## Supported Boards
- EVKB-IMXRT1050
- MIMXRT1060-EVKB
- MIMXRT1170-EVKB
- MIMXRT1060-EVKC
- EVK-MIMXRT1010
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
- FRDM-K32L2B
- FRDM-K32L3A6
- FRDM-MCXA153
- FRDM-MCXA156
- FRDM-MCXA276
- FRDM-MCXC242
- FRDM-MCXC444
- FRDM-MCXN236
- FRDM-MCXN947
- LPCXpresso54628
- LPCXpresso54S018
- LPCXpresso54S018M
- LPCXpresso55S16
- LPCXpresso55S28
- LPCXpresso55S36
- LPCXpresso55S69
- MCX-N5XX-EVK
- MCX-N9XX-EVK
- MIMXRT685-AUD-EVK
- MIMXRT700-EVK
- RD-RW612-BGA
