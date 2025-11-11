Hardware requirements
=====================
- Micro USB cable
- FRDM-MCXN236 board
- Personal Computer

Board settings
============

### Default MCUBoot memory layout - bootloader located in IFR region

| Region         | From       | To         | Size  |
|----------------|------------|------------|-------|
| Primary slot   | 0x00000000 | 0x0007FFFF | 512kB |
| Secondary slot | 0x00080000 | 0x000FFFFF | 512kB |
|----------------|------------|------------|-------|
| MCUboot code   | 0x01008000 | 0x0100FFFF |  32kB |

- MCUBoot resides in the `Bank_1 IFR_0` region to utilize flash remap feature
- MCUBoot header size is set to 1024 bytes
- Signing algorithm is ECDSA-P256 (TinyCrypt)
- Write alignment is 16 bytes
- MCUBoot is configured to use its `DIRECT_XIP` image handling strategy
- Flash swapping HW feature is used for zero-overhead image swapping

Image signing example:

    imgtool sign --key sign-ecdsa-p256-priv.pem
                 --align 16
                 --version 1.1
                 --slot-size 0x80000
                 --header-size 0x400
                 --pad-header
                 ota_mcuboot_basic.bin
                 ota_mcuboot_basic.SIGNED.bin

*Note* that for the first image flashed manually together with the bootloader
additional imgtool options `--pad` and `--confirm` must be used. Otherwise
the bootloader would reject the image for missing data in the trailer area.

If the toolchain used for build has problems with writing the bootloader
to the IFR flash region, an alternative can be to use NXP blhost utility
or *MCUXpresso Secure Provisioning Tool*.

**IMPORTANT:**
The CMPA region must be configured so that ROM loader boots from the IFR
region. This is done by setting the `BOOT_CFG.BOOT_SRC` field to `0b10`. This
value directs the ROM loader to start execution from the IFR region where
MCUboot is located. Default CMPA setup with this particular field set is
attached to this example in `cmpa.bin` binary file.
To flash this configuration together with MCUboot binary the following blhost
commands can be used.  Note that the board must be in the ISP mode to use
the following commands:

    #Erase IFR0 Bank 1 region
    blhost -u 0x1FC9,0x0158 -- flash-erase-region 0x01008000 32768 0

    #Flash MCUboot
    blhost -u 0x1FC9,0x0158 -- write-memory 0x01008000 mcuboot_opensource.bin

    #Flash CMPA
    blhost -u 0x1FC9,0x0158 -- write-memory 0x01004000 cmpa.bin

To flash signed application using blhost you can use the following:

    #Erase first application slot
    blhost -u 0x1fc9,0x0158 -- flash-erase-region 0 0x80000 0

    #Flash signed application (must be signed with --pad --confirm flags)
    blhost -u 0x1FC9,0x0158 -- write-memory 0 ota_mcuboot_basic.N236.SIGNED.PADDED.bin


### Alternative MCUBoot memory layout - bootloader located in the main memory

It is also possible to have the bootloader occupy the main memory.
This allows for larger memory footprint compared to IFR region (32kB). However
the flash remapping (swapping) feature becomes difficult to use in this configuration.
For more details about this mode see MCUboot example for MCXN947 board where both
the default IFR scheme and this alternative one are implemented.

