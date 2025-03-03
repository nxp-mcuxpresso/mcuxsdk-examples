# MCUXpresso OTA

MCUXpresso SDK provides several examples and software to demonstrate OTA capabilities of NXP devices. The set of examples located in `ota_examples` targets to demonstrate usage of MCUBoot or ROM-only based solutions.

## MCUboot examples

[MCUboot](https://docs.mcuboot.com/) is a second stage bootloader. A device typically needs a bootloader to be able to update a firmware and verify its validity before executing it. NXP devices already implement a form of a bootloader in the ROM code. The ROM bootloader often implements similar features like code signature validation or mechanisms for firmware update. If features offered by the ROM bootloader are not enough or a user has some special requirements the typical solution to this is a use of the second stage bootloader. The typical chain of execution is then ROM bootloader → Second stage bootloader → Application.

MCUboot is an open source community project that aims to implement necessary features for firmware update. It's used in the Zephyr project as a default bootloader. In MCUXpresso SDK it's used together with OTA applications to give an example of a solution for firmware update.

Typical structure looks like this:
*	`mcuboot_opensource` - bootloader application, __recommended starting point__
*	`ota_mcuboot_basic` - basic OTA application using a serial line to update
*	`ota_mcuboot_client` - client OTA application using a network connection to update
*	`ota_mcuboot_server` - server OTA application using a network connection to update

`mcuboot_opensource` and `ota_mcuboot_basic` are always supported. Network examples and its variants (ethernet, wifi) depends on target device and current state of the support.

### MCUboot and advanced topics

[MCUboot and flash remapping](flash_remap_readme.md)

[MCUboot and Encrypted XIP](encrypted_xip_readme.md)

[MCUboot and OTA update by using SB3 file](sb3_readme.md)

## ROM bootloader examples

OTA solutions using only the ROM bootloader offer a lightweight solution utilizing minimum memory footprint. The examples demostrates typically dual-image feature (ROM utilizing flash remap) and [SB3](https://spsdk-try.readthedocs.io/en/stable/images/secure_update.html) file as an OTA image.

Currently, there is only one example `ota_rom_server` supported in MCXN9 boards, but more boards are planned for next SDK release.