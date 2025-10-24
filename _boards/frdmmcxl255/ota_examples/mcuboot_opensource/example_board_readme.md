Hardware requirements
=====================
- FRDM-MCXL255 board
- USB-C cable
- Personal Computer

Board settings
==============

### MCUBoot layout

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x00000000 | 0x0000FFFF |   64kB |
| Primary slot   | 0x00010000 | 0x00037FFF |  160kB |
| Secondary slot | 0x00038000 | 0x0005FFFF |  160kB |

- MCUBoot header size is set to 512 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 16 bytes
- Uses image swapping by `SWAP_USING_MOVE`

If you experience problems with flash erase, check correct configuration
of `ACL_SEC_x` fields in CMPA region. Also see notes on flash access control
in main.c source file.

### Image signing example

    imgtool sign   --key sign-ecdsa-p256-priv.pem
                   --align 16
                   --version 1.1
                   --slot-size 0x28000
                   --header-size 0x200
                   --pad-header
                   --max-sectors 32
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin

