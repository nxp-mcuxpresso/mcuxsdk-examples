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

- MCUBoot header size is set to 1024 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 4 bytes
- MCUBoot is configured to use its OVERWRITE_ONLY image handling strategy
- To manually enter serial recovery mode during reset, keep button SW1 pressed

### Image signing example

    imgtool sign   --key sign-ecdsa-p256-priv.pem
                   --align 4
                   --version 1.1
                   --slot-size 0x30000
                   --header-size 0x400
                   --pad-header
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin

NOTE - there is alternative prj.conf file named prj-mbedtls.conf
that can be used to enable mbedtls with HW crypto acceleration.
