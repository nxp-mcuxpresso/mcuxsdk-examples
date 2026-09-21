# MCUXpresso OTA examples

MCUXpresso SDK provides several examples and software components that demonstrate the OTA (Over-The-Air) update capabilities of NXP devices. The set of examples located in `ota_examples` demonstrates the usage of the open-source MCUboot or the ROM bootloader on NXP devices.

['Release notes'](CHANGELOG.md)

## MCUboot examples

[MCUboot](https://docs.mcuboot.com/) is a second stage bootloader. A device typically needs a bootloader to be able to update firmware and verify its validity before executing it. NXP devices already implement a form of bootloader in their ROM code. The ROM bootloader often implements similar features such as code signature validation or mechanisms for firmware updates. If features offered by the ROM bootloader are not sufficient or a user has special requirements, the typical solution is to use a second-stage bootloader. The typical chain of execution is then: ROM bootloader → Second-stage bootloader → Application.

MCUboot is an open-source community project that aims to implement the features necessary for firmware updates. It is used as the default bootloader in the Zephyr project. The MCUXpresso SDK uses an NXP fork of this upstream MCUboot, extended with NXP-specific ports and hardware support, together with the OTA applications to provide an example firmware update solution.

The MCUboot examples are structured as follows:
*	`mcuboot_opensource` - bootloader application, __recommended starting point__
*	`ota_mcuboot_basic` - basic OTA application using a serial line to update
*	`ota_mcuboot_client` - client OTA application using a network connection secured by TLS to update
*	`ota_mcuboot_server` - server OTA application using a network connection to update

The `ota_mcuboot_basic` example is always supported. The network examples and their variants (Ethernet, Wi-Fi) depend on the target device and the current state of its support.

## MCUboot upgrade modes

NXP implements several image handling strategies in `ota_examples` to cover specific use cases. Each upgrade mode maps to a specific MCUboot configuration, which can be found in `mcuboot_config.h`.

Some MCUboot upgrade modes support a so-called revert function. An application can test itself during its first boot and mark itself with a confirmation flag. Otherwise, during the next boot, the bootloader invalidates (deletes) the unconfirmed application and boots the previous version.

The table below gives an overview of the upgrade modes.

| **Upgrade mode** | **slbconfig.h / Kconfig project configuration** | **Revert** | **Note** | 
|---|---|---|---|
| Swap | CONFIG_BOOT_MODE_SWAP | Yes | The mode performs a complex technique that swaps images and allows to revert back to the previous version.<br>It also produces more flash wear and requires that the flash is able to work with incremental writes within erased flash page. |
| Flash remap | CONFIG_BOOT_MODE_FLASH_REMAP | Yes | Image swapping offloaded to HW using flash remapping feature.<br>See ['MCUboot and flash remapping'](_doc/flash_remap_readme.md) |
| Overwrite only | CONFIG_BOOT_MODE_OVERWRITE_ONLY | No | The image in the primary slot is overwritten with the new one in the secondary slot. |
| Encrypted XIP: Overwrite only | CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE | No | Uses modified overwrite-only mode to utilize encrypted XIP.<br>The encrypted image (encrypted offline by imgtool) in the secondary slot is re-encrypted to the primary slot with hardware on-the-fly encryption.<br>See ['MCUboot and Encrypted XIP'](_doc/encrypted_xip.md) |
| Encrypted XIP: Flash remap | CONFIG_BOOT_MODE_ENCRYPTED_XIP_REMAP | Yes | (Experimental) Utilize Encrypted XIP with flash remapping feature. See ['MCUboot and Encrypted XIP'](_doc/encrypted_xip.md) |
| Single application slot | CONFIG_BOOT_MODE_SINGLE_APPLICATION_SLOT | No | This mode is suitable for a device with limited flash memory resource as there is no second slot for image staging.<br>The update can be only done manually by blhost/programmer or from bootloader context using MCUboot's ['serial recovery'](_doc/serial_recovery.md) feature. |



## ROM bootloader examples

A ROM bootloader is a first-stage bootloader embedded in an NXP device that offers a lightweight OTA solution with a minimal memory footprint. If supported, the ROM bootloader (without the second-stage MCUboot) can be evaluated in the `ota_rom_basic` example.

The example typically demonstrates a dual-image feature (the ROM using flash remapping) and an [SB3](https://spsdk-try.readthedocs.io/en/stable/images/secure_update.html) file as the OTA image. More information about SB3 processing can be found in ['OTA update by using SB3 file'](_doc/sb3_common_readme.md).

## State of support and roadmap

The MCUXpresso SDK provides three levels of OTA support for a board:

1. **Basic enablement** - the MCUboot bootloader plus a minimal OTA application (`ota_mcuboot_basic`) that updates over the UART debug console (XMODEM).
2. **Network OTA** - HTTP/TLS based firmware update over Ethernet or Wi-Fi (`ota_mcuboot_client` / `ota_mcuboot_server`) for boards with sufficient resources and connectivity.
3. **Extended HW features** - use of device hardware to improve the OTA solution: crypto acceleration, flash remapping, security such as encrypted XIP or secure binary (SB3).

Every board listed below provides the basic enablement (level 1). The matrix summarizes the level 2 and level 3 OTA features currently enabled in the MCUXpresso SDK examples for each board. It reflects the state of the example projects, not the theoretical hardware capabilities - a feature may be blank simply because it is not enabled in the examples yet. This chapter therefore also serves as a public roadmap: cells marked **Planned** indicate features that are intended to be enabled in a future release.

Legend:
* **Network** - HTTP/TLS based OTA via `ota_mcuboot_client` / `ota_mcuboot_server`. The transport is noted as ENET (Ethernet) and/or Wi-Fi.
* **HW crypto** - Hardware accelerated crypto, enabled through the PSA layer in mbedTLS. It typically accelerates hash calculation, signature validation and decryption (of an encrypted image). Boards without it fall back to a software crypto backend (mbedTLS SW or TinyCrypt).
* **Flash remap** - Zero-copy image swapping using the flash remapping HW feature. See [`_doc/flash_remap_readme.md`](_doc/flash_remap_readme.md).
* **Encrypted XIP** - Execution of encrypted code using on-the-fly (de)cryption HW. See [`_doc/encrypted_xip.md`](_doc/encrypted_xip.md).
* **SB3** - SB3 (Secure Binary 3.1) container processing to securely encapsulate the OTA image, demonstrated in `ota_mcuboot_basic` (`xmodem_sb3` command) or in ROM-bootloader-only `ota_rom_basic` example. See [`_doc/sb3_common_readme.md`](_doc/sb3_common_readme.md).
* **X** - The feature is not supported by the device or on the board (e.g. missing Ethernet/Wi-Fi hardware), so it cannot be enabled. A blank cell, in contrast, means the device could support the feature but it is not enabled in the examples yet.

### i.MX RT

| Board                                                                                                 | Network     | HW crypto    | Flash remap   | Encrypted XIP | SB3 |
|-------------------------------------------------------------------------------------------------------|:-----------:|:------------:|:-------------:|:-------------:|:---:|
| [evkbimxrt1050](../_boards/evkbimxrt1050/ota_examples/mcuboot_opensource/example_board_readme.md)     | ENET        | Yes          | X             | Yes (BEE)     | X   |
| [evkbmimxrt1060](../_boards/evkbmimxrt1060/ota_examples/mcuboot_opensource/example_board_readme.md)   | ENET        | Yes          | Yes (overlay) | Yes (BEE)     | X   |
| [evkcmimxrt1060](../_boards/evkcmimxrt1060/ota_examples/mcuboot_opensource/example_board_readme.md)   | ENET        | Yes          | Yes (overlay) | Yes (BEE)     | X   |
| [evkmimxrt1020](../_boards/evkmimxrt1020/ota_examples/mcuboot_opensource/example_board_readme.md)     |             | Yes          | X             | Yes (BEE)     | X   |
| [evkmimxrt1040](../_boards/evkmimxrt1040/ota_examples/mcuboot_opensource/example_board_readme.md)     | ENET        | Yes          | Yes (overlay) | Yes (BEE)     | X   |
| [evkmimxrt1064](../_boards/evkmimxrt1064/ota_examples/mcuboot_opensource/example_board_readme.md)     | ENET        | Yes          | Yes (overlay) | Yes (BEE)     | X   |
| [frdmimxrt1152](../_boards/frdmimxrt1152/ota_examples/mcuboot_opensource/example_board_readme.md)     | **Planned** | Yes          | Yes (overlay) | Yes (OTFAD)   | X   |
| [evkmimxrt1160](../_boards/evkmimxrt1160/ota_examples/mcuboot_opensource/example_board_readme.md)     | ENET        | Yes          | Yes (overlay) | Yes (OTFAD)   | X   |
| [evkbmimxrt1170](../_boards/evkbmimxrt1170/ota_examples/mcuboot_opensource/example_board_readme.md)   | ENET        | Yes          | Yes (overlay) | Yes (OTFAD)   | X   |
| [evkmimxrt1180](../_boards/evkmimxrt1180/ota_examples/mcuboot_opensource/example_board_readme.md)     | ENET        | Disabled (1) | X             | **Planned**   | X   |
| [frdmimxrt1186](../_boards/frdmimxrt1186/ota_examples/mcuboot_opensource/example_board_readme.md)     | ENET        | Disabled (1) | X             | **Planned**   | X   |
| [evkmimxrt595](../_boards/evkmimxrt595/ota_examples/mcuboot_opensource/example_board_readme.md)       | Wi-Fi       | Yes          | Yes (overlay) |               | X   |
| [evkmimxrt685](../_boards/evkmimxrt685/ota_examples/mcuboot_opensource/example_board_readme.md)       | Wi-Fi       | Yes          | Yes (overlay) |               | X   |
| [mimxrt685audevk](../_boards/mimxrt685audevk/ota_examples/mcuboot_opensource/example_board_readme.md) |             | Yes          | Yes (overlay) |               | X   |
| [mimxrt700evk](../_boards/mimxrt700evk/ota_examples/mcuboot_opensource/example_board_readme.md)       |             | Yes          | Yes (overlay) |               | Yes |
| [frdmimxrt700](../_boards/frdmimxrt700/ota_examples/mcuboot_opensource/example_board_readme.md)       |             | Yes          | Yes (overlay) |               | Yes |
| [mimxrt2660evk](../_boards/mimxrt2660evk/ota_examples/mcuboot_opensource/example_board_readme.md)     | **Planned** | **Planned**  | **Planned**   | **Planned**   | X   |

(1) HW crypto is currently disabled due to build issues with the current MCUboot version and the PSA layer.

### MCX N

| Board                                                                                           | Network | HW crypto | Flash remap | Encrypted XIP | SB3 |
|-------------------------------------------------------------------------------------------------|:-------:|:---------:|:-----------:|:-------------:|:---:|
| [mcxn5xxevk](../_boards/mcxn5xxevk/ota_examples/mcuboot_opensource/example_board_readme.md)     | ENET    | Yes (1)   | Yes (swap)  | Yes (NPX)     | Yes |
| [mcxn9xxevk](../_boards/mcxn9xxevk/ota_examples/mcuboot_opensource/example_board_readme.md)     | ENET    | Yes (1)   | Yes (swap)  | Yes (NPX)     | Yes |
| [frdmmcxn947](../_boards/frdmmcxn947/ota_examples/mcuboot_opensource/example_board_readme.md)   | ENET    | Yes (1)   | Yes (swap)  | Yes (NPX)     | Yes |
| [frdmmcxn947t](../_boards/frdmmcxn947t/ota_examples/mcuboot_opensource/example_board_readme.md) | ENET    | Yes (1)   | Yes (swap)  | **Planned**   | X   |
| [frdmmcxn236](../_boards/frdmmcxn236/ota_examples/mcuboot_opensource/example_board_readme.md)   | X       | Yes (1)   | Yes (swap)  | Yes           | Yes |

(1) In the default configuration MCUboot is placed in the IFR region, which does not have enough space for the PSA crypto driver. The HW accelerated variant can be evaluated in a customized configuration (see [`_doc/kconfig_customization.md`](_doc/kconfig_customization.md) and particular example board readme).

### MCX A

| Board                                                                                         | Network | HW crypto   | Flash remap | Encrypted XIP | SB3 |
|-----------------------------------------------------------------------------------------------|:-------:|:-----------:|:-----------:|:-------------:|:---:|
| [frdmmcxa153](../_boards/frdmmcxa153/ota_examples/mcuboot_opensource/example_board_readme.md) | X       | X           | X           | X             | X   |
| [frdmmcxa156](../_boards/frdmmcxa156/ota_examples/mcuboot_opensource/example_board_readme.md) | X       | X           | X           | X             | X   |
| [frdmmcxa266](../_boards/frdmmcxa266/ota_examples/mcuboot_opensource/example_board_readme.md) | X       | **Planned** | X           | X             | X   |
| [frdmmcxa346](../_boards/frdmmcxa346/ota_examples/mcuboot_opensource/example_board_readme.md) | X       | X           | X           | X             | X   |
| [frdmmcxa366](../_boards/frdmmcxa366/ota_examples/mcuboot_opensource/example_board_readme.md) | X       | **Planned** | X           | X             | X   |

### MCX C

| Board                                                                                         | Network | HW crypto | Flash remap | Encrypted XIP | SB3 |
|-----------------------------------------------------------------------------------------------|:-------:|:---------:|:-----------:|:-------------:|:---:|
| [frdmmcxc162](../_boards/frdmmcxc162/ota_examples/mcuboot_opensource/example_board_readme.md) | X       | X         | X           | X             | X   |

### MCX E

| Board                                                                                         | Network | HW crypto   | Flash remap | Encrypted XIP | SB3 |
|-----------------------------------------------------------------------------------------------|:-------:|:-----------:|:-----------:|:-------------:|:---:|
| [frdmmcxe247](../_boards/frdmmcxe247/ota_examples/mcuboot_opensource/example_board_readme.md) |         | **Planned** | X           | X             | X   |
| [frdmmcxe31b](../_boards/frdmmcxe31b/ota_examples/mcuboot_opensource/example_board_readme.md) |         | Yes (1)     | X           | X             | X   |
| [frdmmcxe32b](../_boards/frdmmcxe32b/ota_examples/mcuboot_opensource/example_board_readme.md) |         | Yes (1)     | X           | X             | X   |

(1) The HW crypto backend is provided by an alternative `psa_alt_files` configuration of the `mcuboot_opensource` example; the default configuration uses a software crypto backend.

### MCX L

| Board                                                                                         | Network | HW crypto   | Flash remap | Encrypted XIP | SB3 |
|-----------------------------------------------------------------------------------------------|:-------:|:-----------:|:-----------:|:-------------:|:---:|
| [frdmmcxl255](../_boards/frdmmcxl255/ota_examples/mcuboot_opensource/example_board_readme.md) | X       | **Planned** | X           | X             |     |

### MCX W (Wireless)

| Board                                                                                     | Network | HW crypto | Flash remap | Encrypted XIP | SB3 |
|-------------------------------------------------------------------------------------------|:-------:|:---------:|:-----------:|:-------------:|:---:|
| [mcxw72evk](../_boards/mcxw72evk/ota_examples/mcuboot_opensource/example_board_readme.md) | X       | Yes       | X           | X             |     |

### KW (Wireless)

| Board                                                                                 | Network | HW crypto | Flash remap | Encrypted XIP | SB3 |
|---------------------------------------------------------------------------------------|:-------:|:---------:|:-----------:|:-------------:|:---:|
| [kw47evk](../_boards/kw47evk/ota_examples/mcuboot_opensource/example_board_readme.md) | X       | Yes       | X           | X             |     |

### RW (Wireless)

| Board                                                                                       | Network     | HW crypto | Flash remap   | Encrypted XIP | SB3 |
|---------------------------------------------------------------------------------------------|:-----------:|:---------:|:-------------:|:-------------:|:---:|
| [frdmrw612](../_boards/frdmrw612/ota_examples/mcuboot_opensource/example_board_readme.md)   | ENET, Wi-Fi | Yes       | Yes (overlay) | Yes (IPED)    | Yes |
| [rdrw612bga](../_boards/rdrw612bga/ota_examples/mcuboot_opensource/example_board_readme.md) | ENET, Wi-Fi | Yes       | Yes (overlay) | Yes (IPED)    | Yes |

### LPC

| Board                                                                                                 | Network | HW crypto | Flash remap | Encrypted XIP | SB3 |
|-------------------------------------------------------------------------------------------------------|:-------:|:---------:|:-----------:|:-------------:|:---:|
| [lpcxpresso55s69](../_boards/lpcxpresso55s69/ota_examples/mcuboot_opensource/example_board_readme.md) | X       | X         | X           | X             | X   |

## Advanced topics

['MCUboot and flash remapping'](_doc/flash_remap_readme.md)

['MCUboot and Encrypted XIP'](_doc/encrypted_xip.md)

['OTA update by using SB3 file'](_doc/sb3_common_readme.md)

['Kconfig and customization of OTA examples'](_doc/kconfig_customization.md)

['Serial recovery'](_doc/serial_recovery.md)