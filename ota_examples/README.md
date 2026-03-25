# MCUXpresso OTA examples

MCUXpresso SDK provides several examples and software to demonstrate OTA capabilities of NXP devices. The set of examples located in `ota_examples` targets to demonstrate usage of opensource MCUBoot or ROM bootloader in NXP devices.

['Release notes'](CHANGELOG.md)

## MCUboot examples

[MCUboot](https://docs.mcuboot.com/) is a second stage bootloader. A device typically needs a bootloader to be able to update firmware and verify its validity before executing it. NXP devices already implement a form of bootloader in their ROM code. The ROM bootloader often implements similar features such as code signature validation or mechanisms for firmware updates. If features offered by the ROM bootloader are not sufficient or a user has special requirements, the typical solution is to use a second-stage bootloader. The typical chain of execution is then: ROM bootloader → Second-stage bootloader → Application.

MCUboot is an open source community project that aims to implement necessary features for firmware update. It's used in the Zephyr project as a default bootloader. In MCUXpresso SDK it's used together with OTA applications to give an example of a solution for firmware update.

MCUboot examples structure looks like this:
*	`mcuboot_opensource` - bootloader application, __recommended starting point__
*	`ota_mcuboot_basic` - basic OTA application using a serial line to update
*	`ota_mcuboot_client` - client OTA application using a network connection secured by TLS to update
*	`ota_mcuboot_server` - server OTA application using a network connection to update

The `ota_mcuboot_basic` is always supported. Network examples and its variants (ethernet, wifi) depends on target device and current state of the support.

### Upgrade modes

NXP implements several image handling strategies in ota_examples to cover specific use cases. Each upgrade mode expands in specific MCUboot configuration which can be found in `mcuboot_config.h`. 

Some upgrade modes of MCUBoot support a so-called revert function. An application can test itself during its first boot and mark itself with a confirmation flag. Otherwise, during the next boot, the bootloader invalidates (deletes) the unconfirmed application and boots the previous version.

Table below shows overview of upgrade modes.

| **Upgrade mode** | **slbconfig.h / Kconfig project configuration** | **Revert** | **Note** | 
|---|---|---|---|
| Swap | CONFIG_BOOT_MODE_SWAP | Yes | The mode performs a complex technique that swaps images and allows to revert back to the previous version.<br>It also produces more flash wear and requires that the flash is able to work with incremental writes within erased flash page. |
| Flash remap | CONFIG_BOOT_MODE_FLASH_REMAP | Yes | Image swapping offloaded to HW using flash remapping feature.<br>See ['MCUboot and flash remapping'](_doc/flash_remap_readme.md) |
| Overwrite only | CONFIG_BOOT_MODE_OVERWRITE_ONLY | No | The image in the primary slot is overwritten with the new one in the secondary slot. |
| Encrypted XIP: Overwrite only | CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE | No | Uses modified overwrite-only mode to utilize encrypted XIP.<br>The encrypted image (encrypted offline by imgtool) in the secondary slot is re-encrypted to the primary slot with hardware on-the-fly encryption.<br>See ['MCUboot and Encrypted XIP'](_doc/encrypted_xip.md) |
| Encrypted XIP: Flash remap | CONFIG_BOOT_MODE_ENCRYPTED_XIP_REMAP | Yes | (Experimental) Utilize Encrypted XIP with flash remapping feature. See ['MCUboot and Encrypted XIP'](_doc/encrypted_xip.md) |
| Single application slot | CONFIG_BOOT_MODE_SINGLE_APPLICATION_SLOT | No | This mode is suitable for a device with limited flash memory resource as there is no second slot for image staging.<br>The update can be only done manually by blhost/programmer or from bootloader context using MCUboot's ['serial recovery'](_doc/serial_recovery.md) feature. |



## ROM bootloader examples

A ROM bootloader is a first stage bootloader embedded in a NXP device which offers a lightweight OTA solution with minimum memory footprint. If supported, the ROM bootloader (without the second stage MCUboot) can be evaluated in `ota_rom_basic` example.

The example typically demostrate a Dual image feature (ROM utilizing flash remap) and a [SB3](https://spsdk-try.readthedocs.io/en/stable/images/secure_update.html) file as an OTA image. More information related to the SB3 processing can be found in ['OTA update by using SB3 file'](_doc/sb3_common_readme.md).

## Advanced topics

['MCUboot and flash remapping'](_doc/flash_remap_readme.md)

['MCUboot and Encrypted XIP'](_doc/encrypted_xip.md)

['OTA update by using SB3 file'](_doc/sb3_common_readme.md)

['Kconfig and customization of OTA examples'](_doc/kconfig_customization.md)

['Serial recovery'](_doc/serial_recovery.md)