# dev_composite_cdc_msc




## Overview

The Composite CDC_MSC project is a simple demonstration program based on the MCUXpresso SDK.  
It is enumerated as a COM port and a RAM disk, which can be opened using terminal tools, such as TeraTerm. 
The demo echoes back any character it receives. The RAM disk can be formatted. 
The purpose of this demo is to show how to build a composite USB device and to provide a simple example for further development.

## System Requirement

### Hardware requirements

- Mini/micro USB cable
- USB A to micro AB cable
- Hardware (Tower module/base board, and so on) for a specific device
- Personal Computer


### Software requirements

- The project files are in: 
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_device_composite_cdc_msc/<rtos>/<toolchain>.
<br> For a lite version, the project files are in: 
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_device_composite_cdc_msc_lite/<rtos>/<toolchain>.
> The <rtos> is Bare Metal or FreeRTOS OS.


## Getting Started

### Hardware Settings

> Set the hardware jumpers (Tower system/base module) to default settings.


### Prepare the example 

1.  Download the program to the target board.
2.  Connect the target board to the external power source (the example is self-powered).
3.  Either press the reset button on your board or launch the debugger in your IDE to begin running
    the demo.
4.  Connect a USB cable between the PC host and the USB device port on the board.

> For detailed instructions, see the appropriate board User's Guide.

## Run the example in Windows OS

1.  A COM port and a USB Mass Storage Device is enumerated in the Device Manager. 
If it prompts for CDC driver installation, see the next section to install the CDC driver.
2.  Open the COM port in a terminal tool, such as the Tera Term.
3.  Type some characters, which are echoed back from the COM port.
<br>![Run virtual com example](run_vcom.jpg "Run virtual com example")
4.  The Windows OS prompts to format the U disk.
<br>![Format disk prompt](format_disk_prompt.jpg "Format disk prompt")
When the format is completed, the computer displays the capacity of 4 KB removable disk.
<br>![Disk present on Windows](display_disk.jpg "Disk present on Windows")


note<br>
-  Because there is no dynamic detection between the host and the device, the COM port must be closed from the terminal tool prior to unplugging the CDC device. 
Otherwise, the CDC device won't get recognized next time that it is plugged in with the COM port still opened.
-  If no hardware FLOW CONTROL is needed, set the variable start_transactions to TRUE.
-  The RAM disk and SD disk function can't be enabled at the same time.
-  The Mac OS system default creates the .fseventsd , a trash folder, and some other files if the disk is formatted on Mac OS. 
The total files size is about 8 K. If the USB mass storage example is running on Mac OS, increase the RAM size to 32 K. 
Change the MACRO `TOTAL_LOGICAL_ADDRESS_BLOCKS_NORMAL` in disk.h from 48 to 64.


## Installing the CDC driver for virtual_com and msd_cdc composite example
Below are the steps to install the CDC driver on Windows 7 OS. Similar process applies to Windows XP OS.

Step 1. Click "Update Driver Software..."

<br>![](cdc_driver_install_00.jpg "")


Step 2. Choose "Browse..."

<br>![](cdc_driver_install_01.jpg "")


Step 3. Select "Let me pick..."

<br>![](cdc_driver_install_02.jpg "")


Step 4. Navigate to your CDC driver location.

<install_dir>\\boards\\<board>\\usb_examples\\usb_device_composite_cdc_msc\\inf

or

<install_dir>\\boards\\<board>\\usb_examples\\usb_device_composite_cdc_msc_lite\\inf


<br>![](cdc_driver_install_03.jpg "")


Step 5. Press "Next".

<br>![](cdc_driver_install_04.jpg "")


Step 6. Ignore the warning and press "Yes".

<br>![](cdc_driver_install_05.jpg "")


Step 7. Now the CDC driver should be installed successfully.

<br>![](cdc_driver_install_06.jpg "")


- If the driver signature issue occurs on Windows 8 OS, see the link,\n
[Disabling Driver Signature on Windows 8](https://learn.sparkfun.com/tutorials/disabling-driver-signature-on-windows-8/disabling-signed-driver-enforcement-on-windows-8)
- To enable driver signing on Windows OS, see the link,
  + <a href="msdn.microsoft.com/en-us/library/windows/hardware/ff544865(v=vs.85.aspx)">Driver Signing</a>
  + [Practical Windows Code and Driver Signing](http://www.davidegrayson.com/signing/#howto)

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
- [FRDM-K32L2A4S](../../_boards/frdmk32l2a4s/usb_examples/usb_device_composite_cdc_msc/example_board_readme.md)
- FRDM-K32L2B
- FRDM-K32L3A6
- FRDM-MCXA156
- FRDM-MCXA276
- FRDM-MCXC444
- FRDM-MCXN236
- FRDM-MCXN947
- FRDM-RW612
- LPCXpresso51U68
- LPCXpresso54628
- LPCXpresso54S018
- LPCXpresso54S018M
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/usb_examples/usb_device_composite_cdc_msc/example_board_readme.md)
- LPCXpresso55S28
- LPCXpresso55S36
- LPCXpresso55S69
- MCX-N5XX-EVK
- MCX-N9XX-EVK
- MIMXRT685-AUD-EVK
- MIMXRT700-EVK
- RD-RW612-BGA
