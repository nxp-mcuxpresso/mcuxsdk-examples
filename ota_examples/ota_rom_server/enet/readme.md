# ota_rom_server_enet

## Overview

This application example demonstrates an OTA update using the ROM API and SB
image format. File transfer is done using a simple HTTP server running on the
board.

## Supported Boards
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/ota_examples/ota_rom_server/enet/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/ota_examples/ota_rom_server/enet/example_board_readme.md)

## Prepare the Demo

1. Connect a USB cable between the PC and the board, make sure the power is on.
2. Open a serial terminal on PC for OpenSDA serial(or USB to Serial) device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Insert the Ethernet Cable into the target board's RJ45 port and connect it to your PC network adapter.
4. Configure the PC's IP to be in the network 192.168.0.0, so for example 192.168.0.100

## Running the demo

1.  Once the example is executed, you should see the following output in the console:

	    Initializing PHY...

	    ************************************************
	    ROM OTA HTTP Server Example
	    Built Jan 26 2024 15:15:52
	    ************************************************
	     IPv4 Address     : 192.168.0.102
	     IPv4 Subnet mask : 255.255.255.0
	     IPv4 Gateway     : 192.168.0.100
    	************************************************

2. Open web browser and type `http://192.168.0.102` (IP address of the board) on the browser address bar.
   The browser should show the main web page of the example.
   Note: Make sure to include "http" protocol specifier in the address bar, so that your browser uses port 80

3. Go to OTA page; Here you can upload SB file with update and see current image state

4. Select SB file with image update and upload it

5. If the update was successful you should instantly see change in the image state, otherwise see console log for more details

6. You can now reset the board to let ROM boot the newly installed image

Known issue:
Browser may not display progress of the upload. This is an issue of the browser rather than OTA demo.
It is possible to watch upload progress in the attached serial console.

## Modifying content of static web pages

To modify content available through the web server you must complete following steps:
  1. Modify, add or delete files in folder `boards\<board_name>\lwip_examples\lwip_httpsrv_ota\webpage`.
  2. Run the script file `middleware\lwip\src\apps\httpsrv\mkfs\mkfs.pl <directory name>` to generate new `httpsrv_fs_data.c`.
     Make sure to execute it from a folder where the file `httpsrv_fs_data.c` is. For example:
        
         C:\sdk\boards\<board_name>\lwip_examples\lwip_httpssrv_ota> C:\sdk\middleware\lwip\src\apps\httpsrv\mkfs\mkfs.pl webpage

  3. Make sure the `httpsrv_fs_data.c` file has been overwritten with the newly generated content.
  4. Re-compile the HTTP server application example and download it to your board. 

