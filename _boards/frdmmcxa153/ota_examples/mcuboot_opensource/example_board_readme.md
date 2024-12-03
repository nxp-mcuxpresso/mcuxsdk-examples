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
| MCUboot code   | 0x00000000 | 0x00006000 |   24kB |
| Primary slot   | 0x00006000 | 0x00011FFF |   48kB |
| Secondary slot | 0x00012000 | 0x0001DFFF |   48kB |
| Optional data  | 0x0001E000 | 0x0001FFFF |    8kB |

- MCUBoot header size is set to 512 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 16 bytes
- Used `OVERWRITE_ONLY` image strategy due to limited flash size
- A large sector size of 8kB needs to be considered when allocating
  space for the image trailer as this needs to be sector aligned.
  This reduces the usable space for the application to only 40kB.

### Image signing example

    imgtool sign   --key sign-ecdsa-p256-priv.pem
                   --align 16
                   --version 1.1
                   --slot-size 0xc000
                   --header-size 0x200
                   --pad-header
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin

If you experience problems with flash erase, check correct configuration
of `ACL_SEC_x` fields in CMPA region.

