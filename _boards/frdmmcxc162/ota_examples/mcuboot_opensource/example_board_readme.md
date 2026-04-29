Hardware requirements
=====================
- FRDM-MCXC162 Board
- USB-C cable
- Personal Computer

Board settings
==============

### MCUBoot layout

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x00000000 | 0x00007FFF |   32kB |
| Primary slot   | 0x00008000 | 0x0000FFFF |   32kB |

- MCUBoot header size is set to 512 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 16 bytes
- Uses `SINGLE_APPLICATION_SLOT` image strategy due to limited flash size
- To manually enter serial recovery mode, keep the SW2 button pressed during reset, then release it

### Image signing example

    imgtool sign --key sign-ecdsa-p256-priv.pem
                 --align 16
                 --version 1.1
                 --slot-size 0x8000
                 --header-size 0x200
                 --max-sectors 4
                 --pad-header
                 ota_mcuboot_basic.bin
                 ota_mcuboot_basic.SIGNED.bin
