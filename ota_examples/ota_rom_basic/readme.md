# ota_rom_basic

## Overview
This `ota_rom_basic` example demonstrates a basic application that uses ROM as a first stage bootloader. A secure binary (SB) is used as an image for OTA update.

## Prepare the Demo
1. __The demo requires provisioned device to function properly.
   Please refer to respective readme for your target board referenced in `sb3_common_readme.md` located in docs folder and follow the steps there before you continue.__
2. Connect a USB cable between the PC and the Debug USB port on the target board.
3. Open a serial terminal on PC for connected board with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
    - Unix line ending using `\n` (LF)

## Transfering data to the flash memory
There are multiple ways how to transfer image updates to the flash memory:

- This examples implements a simple XMODEM-CRC protocol, that can be used to transfer data to the board at slow speed (~10kB/s).
  This provides a convenient method for images that have relatively small size (under lower 100's of kB).
  In the application shell the XMODEM receiving is initiated by `xmodem_sb3` command. Terminal emulators like Tera Term or ExtraPutty
  can be used as a transmitting side. Both 128B and 1024B packet sizes are supported.
- An alternative way, more suitable for larger images, is to use the *blhost* utility (part of NXP's SPSDK and MCUXpresso Secure Provisioning Tool).
  Blhost communicates with chip's ROM and supports all basic flash operations. When used via onboard's USB the transfer
  speed is more appropriate for larger files. See board's user manual for details on ISP mode.
- Another option is to use a debug adapter (e.g. JLink, CMSIS DAP...) and flash data using their tools.


## Running the demo
To get the application properly executed by the ROM bootloader, it is necessary to put signed application image to the image region partition.
There are multiple options how to achieve that, however in principle the are two main methods (both presuming the device is correctly provisioned):

1.  programing signed application image to the primary application partition using an external tool (direct method)
2.  jump-starting the application by debugger, performing an image update with the signed image, resetting the board and letting the bootloader to perform the update (indirect method)

**The latter method is used in the following step-by-step description:**

1.  Open the demo project and build it.
    
2.  Prepare SB file of the application from raw binary as described in the `sb3_[&device]_readme.md` for you board.

3.  Launch the debugger in your IDE to jump-start the application.

4.  When the demo starts successfully, the terminal will display shell prompt as in the following example:

        *************************************
        *  Basic ROM application example    *
        *************************************

        Built Feb 22 2025 14:09:14

        $

5.  Available commands can be printed with `help` command

6.  Current image state is printed with `image` command. The output will reflect settings of the used platform and
    may not be exactly the same as the following output:

        $ image
        IMAGE 0: Invalid image header 
		IMAGE 1: Invalid image header   	

7.  From the shown `image` command output it can be observed that there are currently no signed images present neither
    in the image 0 nor image 1 region.  Since the example was started by a debugger, it is not signed and hence
    not recognized as a valid signed image in the image 0 region.

8.  XMODEM transfer is initiated using `xmodem_sb3` command. When executed, it starts waiting for the transmitting side.
    In this demonstration Tera Term is used to send the update file. The dialog `File->Transfer->XMODEM` is used
    to select a file to be send. In the same dialog a packet size can be extended to 1kB for faster transmittion.

        $ xmodem_sb3
        Initiated XMODEM-CRC transfer. Receiving... (Press 'x' to cancel)
        CCC
        Received 41984 bytes
        SB3 has been processed

10. After image downloaded finished the current image status can be checked with `image` command. The output now
    shows that there is an image file detected in the image 1 region.

        $ image
        IMAGE 0:Invalid image header 
		IMAGE 1:
			<IMG_VERSION 0x1 LENGTH 34356 EXEC_ADDR 0x8001000>

11. Running `reboot` command or resetting the board manually starts the bootloader and lets it handle the installation
    of new image. If everything went well the image in the image 1 region is validated and marked for boot. The `image` command output should reflect this change:

        $ image
        IMAGE 0:Invalid image header 
		IMAGE 1:
			<IMG_VERSION 0x1 LENGTH 34356 EXEC_ADDR 0x8001000>
			*ACTIVE*