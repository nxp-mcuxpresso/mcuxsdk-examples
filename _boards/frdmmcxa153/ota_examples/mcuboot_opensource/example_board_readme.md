Hardware requirements
=====================
- FRDM-MCXA153 board
- USB-C cable
- Personal Computer

Board settings
==============

### MCUBoot layout

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x00000000 | 0x00007FFF |   32kB |
| Primary slot   | 0x00008000 | 0x0001FFFF |   96kB |

- MCUBoot header size is set to 512 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 16 bytes
- Used `SINGLE_IMAGE` image strategy due to limited flash size

### Image signing example

    imgtool sign   --key sign-ecdsa-p256-priv.pem
                   --align 16
                   --version 1.1
                   --slot-size 0x18000
                   --header-size 0x200
                   --pad-header
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin

If you experience problems with flash erase, check correct configuration
of `ACL_SEC_x` fields in CMPA region.

