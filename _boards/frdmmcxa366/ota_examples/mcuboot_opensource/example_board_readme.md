Hardware requirements
=====================
- FRDM-MCXA366 board
- USB-C cable
- Personal Computer

Board settings
==============

### MCUBoot layout

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x00000000 | 0x0000BFFF |   48kB |
| Primary slot   | 0x0000C000 | 0x0007DFFF |  456kB |
| Secondary slot | 0x0007E000 | 0x000EFFFF |  456kB |

- MCUBoot header size is set to 512 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 16 bytes
- Uses image swapping by `SWAP_USING_MOVE`
- To manually enter serial recovery mode, keep the SW2 button pressed during reset, then release it

### Image signing example

    imgtool sign   --key sign-ecdsa-p256-priv.pem
                   --align 16
                   --version 1.1
                   --slot-size 0x70000
                   --header-size 0x200
                   --pad-header
                   --max-sectors 64
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin

If you experience problems with flash erase, check correct configuration
of `ACL_SEC_x` fields in CMPA region.
