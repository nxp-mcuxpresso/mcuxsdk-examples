Hardware requirements
=====================
- FRDM-MCXA156 board
- USB-C cable
- Personal Computer

Board settings
==============
MCUBoot layout is the following:

  MCUBoot region       0x0     - 0x08000  : 32kB
  Primary slot         0x8000  - 0x84000  : 496kB (0x7c000 bytes)
  Secondary slot       0x84000 - 0x100000 : 496kB (0x7c000 bytes)  

- MCUBoot header size is set to 512 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 16 bytes
- Uses image swapping by `SWAP_USING_MOVE`

Image signing example:

  imgtool sign --key sign-ecdsa-p256-priv.pem
               --align 16
               --version 1.1
               --slot-size 0x7c000
               --header-size 0x200
               --pad-header
	       --max-sectors 64
               ota_mcuboot_basic.bin
               ota_mcuboot_basic.SIGNED.bin

If you experience problems with flash erase, check correct configuration
of `ACL_SEC_x` fields in CMPA region.

