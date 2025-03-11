Hardware requirements
=====================
- MIMXRT700 board
- Mini/micro USB cable
- Personal Computer

Board settings
============

### MCUBoot layout

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x28000000 | 0x2803FFFF | 256kB  |
| Primary slot   | 0x28040000 | 0x2823FFFF | 2048kB |
| Secondary slot | 0x28240000 | 0x2843FFFF | 2048kB |

- MCUBoot header size is set to 1024 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 4 bytes
- MCUBoot is configured to use its `SWAP` image handling strategy

### Image signing example

    imgtool sign   --key sign-ecdsa-p256-priv.pem
                   --align 4
                   --version 1.1
                   --slot-size 0x200000
                   --header-size 0x400
                   --pad-header
                   --max-sectors 800
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin
