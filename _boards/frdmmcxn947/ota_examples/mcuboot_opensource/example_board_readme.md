Hardware requirements
=====================
- Micro USB cable
- FRDM-MCXN947 board
- Personal Computer

Board settings
============
MCUBoot layout for the internal flash is the following:

  MCUBoot region       0x0100_8000 - 0x0100_FFFF : 32kB
  Primary slot         0x0000_0000 - 0x0010_0000 : 1024kB
  Secondary slot       0x0010_0000 - 0x0200_0000 : 1024kB

- MCUBoot resides in the Bank_1 IFR_0 region
- MCUBoot header size is set to 1024 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 16 bytes
- MCUBoot is configured to use its DIRECT_XIP image handling strategy
- Flash swapping HW feature is used for zero-overhead image swapping

Image signing example:

  imgtool sign --key sign-ecdsa-p256-priv.pem
               --align 16
               --version 1.1
               --slot-size 0x100000
               --header-size 0x400
               --pad-header
               ota_mcuboot_basic.bin
               ota_mcuboot_basic.SIGNED.bin

Note that for the first image flashed manually together with the bootloader
additional imgtool options "--pad" and "--confirm" must be used. Otherwise
the bootloader would reject the image for missing data in the trailer area.

If the toolchain used for build has problems with writing the bootloader
to the IFR flash region, an alternative can be to use NXP blhost utility
or the MCUXpresso Secure Provisioning Tool.

IMPORTANT:
The CMPA region must be configured so that ROM loader boots from the IFR
region. This is done by setting the BOOT_CFG.BOOT_SRC field to 0b10. This
value directs the ROM loader to start execution from the IFR region where
MCUboot is located. Default CMPA setup with this particular field set is
attached to this example in cmpa.bin binary file.
To flash this configuration together with MCUboot binary the following blhost
commands can be used.  Note that the board must be in the ISP mode to use
the following commands:

  - #Erase IFR0 Bank 1 region
  - blhost -u 0x1FC9,0x014F -- flash-erase-region 0x01008000 32768 0

  - #Flash MCUboot
  - blhost -u 0x1FC9,0x014F -- write-memory 0x01008000 mcuboot_opensource.bin

  - #Flash CMPA
  - blhost -u 0x1FC9,0x014F -- write-memory 0x01004000 cmpa.bin
