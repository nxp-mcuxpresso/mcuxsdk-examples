Hardware requirements
===================
- Mini/micro USB cable
- MIMXRT1060-EVKC board
- Personal Computer

Board settings
==============

### MCUBoot layout

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x60000000 | 0x6003FFFF | 256kB  |
| Primary slot   | 0x60040000 | 0x6023FFFF | 2048kB |
| Secondary slot | 0x60440000 | 0x6083FFFF | 2048kB |

- MCUBoot header size is set to 1024 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 4 bytes
- MCUBoot is configured to use its DIRECT_XIP image handling strategy
- Flash remapping HW feature is used for zero-overhead image swapping
- To manually enter serial recovery mode, keep the user button pressed during reset, then release it


### Image signing example

    imgtool sign   --key sign-ecdsa-p256-priv.pem
                   --align 4
                   --version 1.1
                   --slot-size 0x200000
                   --header-size 0x400
                   --pad-header
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin

Note that for the first image flashed manually together with the bootloader
additional imgtool options "--pad" and "--confirm" must be used. Otherwise
the bootloader would reject the image for missing data in the trailer area.

### Custom configuration - Encrypted XIP

| Region              | From       | To         | Size   |
|---------------------|------------|------------|--------|
| MCUboot code        | 0x60000000 | 0x6003FFFF | 256kB  |
| Primary slot        | 0x60040000 | 0x6023FFFF | 2048kB |
| Secondary slot      | 0x60240000 | 0x6043FFFF | 2048kB |
| Encryption metadata | 0x60440000 | 0x60440FFF | 4kB    |

This custom build generates a project with predefined configuration to utilize Encrypted XIP mode. For more information please see [Encrypted XIP and MCUboot](../../../../ota_examples/_doc/encrypted_xip.md).

- MCUBoot header size is set to 1024 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 4 bytes
- MCUBoot is configured for `Encrypted XIP mode` using modified `MCUBOOT_OVERWRITE_ONLY` image update strategy
- MCUboot uses ECDH-P256 to secure AES key for image encryption
- Primary slot is encrypted by BEE module to utilize encrypted XIP

Image signing example:

    imgtool sign --key sign-ecdsa-p256-priv.pem
                 --align 4
                 --version 1.1
                 --slot-size 0x200000
                 --header-size 0x400
                 --max-sectors 59
                 --pad-header
                 -E enc-ec256-pub.pem
                 ota_mcuboot_basic.bin
                 ota_mcuboot_basic.SIGNED.bin

Project building example:

The project is built using `west` tool. For more information please see [MCUXpresso SDK documentation](https://mcuxpresso.nxp.com/mcuxsdk/latest/html/introduction/README.html).

Bootloader:
~~~
west build -p always examples/ota_examples/mcuboot_opensource --toolchain iar --config flexspi_nor_debug -b evkcmimxrt1060 -t guiproject -DCONF_FILE="examples/ota_examples/_custom_cfg/rt10xx/mcuboot_opensource.conf" -d builds/mcuboot
~~~

OTA application:
~~~
west build -p always examples/ota_examples/ota_mcuboot_basic --toolchain iar --config flexspi_nor_debug -b evkcmimxrt1060 -t guiproject -DCONF_FILE="examples/ota_examples/_custom_cfg/rt10xx/ota_mcuboot_basic.conf" -d builds/ota_mcuboot_basic
~~~