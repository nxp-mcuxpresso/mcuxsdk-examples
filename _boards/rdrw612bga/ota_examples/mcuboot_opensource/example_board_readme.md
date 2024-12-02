Hardware requirements
=====================
- Mini/micro USB cable
- Personal Computer

Board settings
============
Make sure the board is setup to boot from flash.

The OTA flash layout is defined in flash_partitioning.h.

In all cases, the MCUBOOT bootloader reserves 128kB at the beginning of the external flash
followed by 4.4MB slots for application.
The resulting layout for the monolithic application will be as follows:
- BOOTLOADER:  0x020000 bytes @ 0x08000000
- APP_ACT:     0x440000 bytes @ 0x08020000
- APP_CAND:    0x440000 bytes @ 0x08460000

For testing purposes, the image authentication may be disabled in sblconfig.h by uncommenting the OTA_TEST definition so that
the following are defined:
   #define MCUBOOT_NO_SIGN
   #define CONFIG_BOOT_HASH_NO_SIGN
   #define CONFIG_BOOT_DIGEST_TYPE_SHA256
