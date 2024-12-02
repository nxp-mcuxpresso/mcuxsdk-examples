Hardware requirements
=====================
- FRDM-MCXA153 board
- USB-C cable
- Personal Computer

Board settings
==============
MCUBoot layout is the following:

  MCUBoot region       0x0     - 0x06000 : 24kB
  Primary slot         0x6000  - 0x12000 : 48kB (0xc000 bytes)
  Secondary slot       0x12000 - 0x1e000 : 48kB (0xc000 bytes)
  Optional user data   0x1e000 - 0x20000 : 8kB

- MCUBoot header size is set to 512 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 16 bytes
- Used OVERWRITE_ONLY image strategy due to limited flash size

Image signing example:

  imgtool sign --key sign-ecdsa-p256-priv.pem
               --align 16
               --version 1.1
               --slot-size 0xc000
               --header-size 0x200
               --pad-header
               ota_mcuboot_basic.bin
               ota_mcuboot_basic.SIGNED.bin

If you experience problems with flash erase, check correct configuration
of ACL_SEC_x fields in CMPA region.

