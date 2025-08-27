Hardware requirements
=====================
- MCX-W72-EVK Board
- USB-C cable
- Personal Computer

Board settings
==============

### MCUBoot layout

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x00000000 | 0x0003FFFF |  256kB |
| Primary slot   | 0x00040000 | 0x0011FFFF |  896kB |
| Secondary slot | 0x00120000 | 0x001FFFFF |  896kB |

- MCUBoot header size is set to 512 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 16 bytes
- Uses image swapping by `SWAP_USING_MOVE`

### Image signing example

    imgtool sign --key sign-ecdsa-p256-priv.pem
                 --align 16
                 --version 1.1
                 --slot-size 0xE0000
                 --header-size 0x400
                 --max-sectors 111
                 --pad-header
                 -E enc-ec256-pub.pem
                 ota_mcuboot_basic.bin
                 ota_mcuboot_basic.SIGNED.bin
