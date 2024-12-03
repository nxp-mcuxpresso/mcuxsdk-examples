Hardware requirements
=====================
- LPCXpresso 55S69 board
- Mini/micro USB cable
- Personal Computer

Board settings
============
No special HW settings are required.

In case of MCUBoot for LPC55S69 there is currently no image revert mechanism.
This is due to Flash ECC capability that doesn't allow incremental flash programming.
MCUBoot's revert strategies rely on this mechanism, therefore it's not currently practical
to use any of the MCUBoot's revert strategies like image swapping by move or stratch area.

### MCUBoot memory layout

| Region         | From       | To         | Size  |
|----------------|------------|------------|-------|
| MCUboot code   | 0x00000000 | 0x0001FFFF | 128kB |
| Primary slot   | 0x00020000 | 0x0004FFFF | 192kB |
| Secondary slot | 0x00050000 | 0x0007FFFF | 192kB |

### Image signing example

    imgtool sign   --key sign-rsa2048-priv.pem
                   --align 4
                   --version 1.1
                   --slot-size 0x30000
                   --header-size 0x400
                   --pad-header
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin

