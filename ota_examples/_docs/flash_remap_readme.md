# MCUboot and flash remapping feature

The default upgrade mechanism in MCUBoot is the SWAP algorithm. There are several NXP processors which support flash remapping functionality that can be used to speed up the OTA update process and minimize the flash wear. Flash remapping feature is used for zero-copy image swapping without the need of any data moving operations. This results in the fastest update operation possible. There are several platforms that support this mechanism. Mostly based on their external memory FlexSPI controller (eg. RT1060, RW612...). Platforms like MCX N9 also implement this mechanism for their internal flash memory.

The boards with such processors have example projects configured to use this feature. This is achieved by using MCUboot's [DIRECT-XIP](https://docs.mcuboot.com/design.html#direct-xip) mechanism and by activating flash remapping when an active flag is located on the secondary slot - image is still built to run from primary slot. Keep in mind that DIRECT-XIP mode loads image with the highest version as there is no rollback support.

**IMPORTANT NOTE:**
Signed application images directly programmed into flash memory by a programmer require additional "--pad --confirm" parameter for [imgtool](https://docs.mcuboot.com/imgtool.html). This parameter adds additional trailer to the signed image and is required by bootloader DIRECT-XIP process. Signed images used in OTA process do not require "-pad" parameter.

## Flash remap mechanisms in NXP devices

There are currently two flash remapping mechanisms:
1. Flash remap using overlay
    * active region overlays inactive region - __inactive region is logically not accessible for read__ - for flash reads user has to manually use a peripheral driver of the memory device
    * OTA process has to download the OTA image into primary or secondary slot depending on the active flag location
2. Flash remap using swap
    * active region swaps address range with inactive region - __both regions are logically accessible__
    * OTA process always downloads the OTA image into secondary (candidate) slot as the primary slot always "virtually" holds the active flag

## Supported Boards

__Flash remap using overlay__

- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/ota_examples/mcuboot_opensource/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/ota_examples/mcuboot_opensource/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/ota_examples/mcuboot_opensource/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/ota_examples/mcuboot_opensource/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/ota_examples/mcuboot_opensource/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/ota_examples/mcuboot_opensource/example_board_readme.md)

__Flash remap using swap__

- [FRDM-MCXN947](../../_boards/frdmmcxn947/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/ota_examples/mcuboot_opensource/example_board_readme.md)