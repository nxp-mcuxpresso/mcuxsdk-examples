Hardware requirements
=====================
- FRDM-MCXE31B board
- USB-C cable
- Personal Computer

Board settings
==============

### MCUBoot layout

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x00401400 | 0x004C0000 |  763kB |
| Primary slot   | 0x00500000 | 0x005C0000 |  768kB |
| Secondary slot | 0x00600000 | 0x006C0000 |  768kB |

- MCUBoot header size is set to 1024 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 8 bytes
- Uses image swapping by `SWAP_USING_MOVE`
- The swapping algorithm requires one extra sector in primary slot
  plus one sector for the image trailer therefore the effective
  application image size is reduced by two sectors (768kB - 16kB)

### Image signing example

    imgtool sign   --key sign-ecdsa-p256-priv.pem
                   --align 8
                   --version 1.1
                   --slot-size 0xC0000
                   --header-size 0x400
                   --pad-header
                   --max-sectors 100
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin

