Hardware requirements
=====================
- FRDM-MCXA156 board
- USB-C cable
- Personal Computer

Board settings
==============

### MCUBoot layout

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x00000000 | 0x00010000 |   64kB |
| Primary slot   | 0x00010000 | 0x000C8000 |  736kB |
| Secondary slot | 0x000C8000 | 0x00018000 |  736kB |

- MCUBoot header size is set to 1024 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 8 bytes
- Uses image swapping by `SWAP_USING_MOVE`

### Image signing example

    imgtool sign   --key sign-ecdsa-p256-priv.pem
                   --align 8
                   --version 1.1
                   --slot-size 0xc8000
                   --header-size 0x400
                   --pad-header
                   --max-sectors 200
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin

