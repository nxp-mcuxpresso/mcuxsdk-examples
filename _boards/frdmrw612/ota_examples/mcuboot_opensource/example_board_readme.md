Hardware requirements
=====================
- Mini/micro USB cable
- Personal Computer

Board settings
============
Make sure the board is setup to boot from flash.

### Default MCUBoot memory layout

In all cases, the MCUBOOT bootloader reserves 256kB at the beginning of the external flash
followed by 2MB slots for application.
The resulting layout for the monolithic application will be as follows:

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x08000000 | 0x0803FFFF |  256kB |
| Primary slot   | 0x08040000 | 0x0821FFFF | 2048kB |
| Secondary slot | 0x08240000 | 0x0841FFFF | 2048kB |


- MCUBoot header size is set to 1024 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 4 bytes
- MCUBoot is configured to use its `DIRECT_XIP` image handling strategy together with FlexSPI flash remapping
- For testing purposes, the image authentication may be disabled in sblconfig.h by uncommenting the `CONFIG_BOOT_OTA_TEST` definition so that
  the following is defined:

    #define MCUBOOT_NO_SIGN
    #define CONFIG_BOOT_HASH_NO_SIGN
    #define CONFIG_BOOT_DIGEST_TYPE_SHA256


### Image signing example

    imgtool sign   --key sign-ecdsa-p256-priv.pem
                   --align 4
                   --version 1.1
                   --slot-size 0x200000
                   --header-size 0x400
                   --pad-header
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin

**Note** that for the first image flashed manually together with the bootloader
additional imgtool options `--pad` and `--confirm` must be used. Otherwise
the bootloader would reject the image for missing data in the trailer area.

### Custom configuration - Encrypted XIP

| Region              | From       | To         | Size   |
|---------------------|------------|------------|--------|
| MCUboot code        | 0x08000000 | 0x0803FFFF |  256kB |
| Primary slot        | 0x08040000 | 0x0821FFFF | 2048kB |
| Secondary slot      | 0x08240000 | 0x0841FFFF | 2048kB |
| Encryption metadata | 0x08440000 | 0x08440FFF |    4kB |

This custom build generates a project with predefined configuration to utilize Encrypted XIP mode. For more information please see [Encrypted XIP and MCUboot](../../../../ota_examples/_doc/encrypted_xip_readme.md).

- MCUBoot header size is set to 1024 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 4 bytes
- MCUBoot is configured for `Encrypted XIP mode` using modified `MCUBOOT_OVERWRITE_ONLY` image update strategy
- MCUboot uses ECDH-P256 to secure AES key for image encryption
- Primary slot is encrypted by IPED module to utilize encrypted XIP

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
west build -p always examples/ota_examples/mcuboot_opensource --toolchain iar -t guiproject --config flash_debug -b frdmrw612 -DCONF_FILE="examples/ota_examples/_custom_cfg/rw61x/mcuboot_opensource.conf" -d builds/rw612_mcuboot
~~~

OTA application:
~~~
west build -p always examples/ota_examples/ota_mcuboot_basic --toolchain iar -t guiproject --config flash_debug -b frdmrw612 -DCONF_FILE="examples/ota_examples/_custom_cfg/rw61x/ota_mcuboot_basic.conf" -d builds/rw612_ota_mcuboot_basic
~~~