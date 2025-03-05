# OTA update by using SB3 file

The [secure binary (SB)](https://spsdk-try.readthedocs.io/en/stable/images/secure_update.html) container in version 3.1 (SB3 for short) provides an alternative way to encapsulate an OTA image. This provides a secure and easy way to use hardware security features of a device to securely transport an OTA update regardless security of transport channel.

In MCUXpresso OTA examples there are two ways of SB3 usage:

   1. __The ROM bootloader utilizing Dual image feature__
      * flash memory is divided to two equal regions for images - based on image version the ROM bootloader disables or enables flash remapping feature and jumps to the application
      * can be evaluated in `ota_rom_basic` example
   2. __The SB3 is used as an encapsulation of MCUboot image to secure its transport__
      * `ota_mcuboot_basic` example provides an additional command `xmodem_sb3` to receive and process the SB3 file

## Risks and flash remapping functionality


Flash remapping feature, if supported, is enabled by default in the examples, however, there are few things to keep in mind when using SB3 as an OTA update.

User has to ensure that the processed SB3 by an application has correct load address targeting currently __inactive image region__ because the active application has no control over ROM IAP handling SB3. This creates a risk with overwriting the active image region leading to bricked device if an incorrect SB3 is downloaded and no backup image is present in the inactive image region to recover. This applies for MCUboot or ROM bootloader use case.

__In other words, two separate SB3 files (for first and second image region) have to be generated if flash remap based on OVERLAY is used. The application has to distinguish these two binaries during an OTA update.__ 

__In case of flash remap based on SWAP mechanism the SB3 file is always generated for second image region. Due design of SWAP the SB3 payload is always written to the inactive image region.__

For more information, please read [Flash remapping functionality](flash_remap_readme.md).

## Step-by-step walkthrough

- [RW61X](sb3_rw61x_readme.md)
- [MCXN](sb3_mcxn_readme.md)

## Supported Boards

- [RD-RW612-BGA](../../_boards/rdrw612bga/ota_examples/mcuboot_opensource/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/ota_examples/mcuboot_opensource/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/ota_examples/mcuboot_opensource/example_board_readme.md)