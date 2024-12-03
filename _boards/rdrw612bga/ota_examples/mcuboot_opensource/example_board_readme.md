Hardware requirements
=====================
- Mini/micro USB cable
- Personal Computer

Board settings
============
Make sure the board is setup to boot from flash.

### MCUBoot memory layout

In all cases, the MCUBOOT bootloader reserves 128kB at the beginning of the external flash
followed by 4.4MB slots for application.
The resulting layout for the monolithic application will be as follows:

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x08000000 | 0x0801FFFF |  128kB |
| Primary slot   | 0x08020000 | 0x0845FFFF | 4352kB |
| Secondary slot | 0x08460000 | 0x0889FFFF | 4352kB |


- MCUBoot is configured to use its `DIRECT_XIP` image handling strategy together with FlexSPI flash remapping
- For testing purposes, the image authentication may be disabled in sblconfig.h by uncommenting the `CONFIG_BOOT_OTA_TEST` definition so that
  the following is defined:

    #define MCUBOOT_NO_SIGN
    #define CONFIG_BOOT_HASH_NO_SIGN
    #define CONFIG_BOOT_DIGEST_TYPE_SHA256


### Image signing example

    imgtool sign   --key sign-rsa2048-priv.pem
                   --align 4
                   --version 1.1
                   --slot-size 0x440000
                   --header-size 0x400
                   --pad-header
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin

**Note** that for the first image flashed manually together with the bootloader
additional imgtool options `--pad` and `--confirm` must be used. Otherwise
the bootloader would reject the image for missing data in the trailer area.
