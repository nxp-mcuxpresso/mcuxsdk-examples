Hardware requirements
=====================
- MIMXRT2660-EVK board
- Mini/micro USB cable
- Personal Computer

Board settings
============
Make sure the board is setup to boot from flash.

### MCUBoot layout

In all cases, the MCUBOOT bootloader reserves 256kB at the beginning of the external flash followed by 2MB slots for application.

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x60000000 | 0x2803FFFF | 256kB  |
| Primary slot   | 0x60040000 | 0x2823FFFF | 2048kB |
| Secondary slot | 0x60240000 | 0x2843FFFF | 2048kB |

- MCUBoot header size is set to 1024 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 4 bytes
- MCUBoot is configured to use its `SWAP` image handling strategy together with FlexSPI flash remapping

### Image signing example

    imgtool sign   --key sign-ecdsa-p256-priv.pem
                   --align 4
                   --version 1.1
                   --slot-size 0x200000
                   --header-size 0x400
                   --pad-header
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin

