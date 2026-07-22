Hardware requirements
=====================
- Micro USB cable
- FRDM-MCXN947T board
- Personal Computer

Board settings
============

### Default MCUBoot memory layout - bootloader located in IFR region

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| Primary slot   | 0x00000000 | 0x000FFFFF | 1024kB |
| Secondary slot | 0x00100000 | 0x001FFFFF | 1024kB |
|----------------|------------|------------|--------|
| MCUboot code   | 0x01008000 | 0x0100FFFF |   32kB |

- MCUBoot resides in the `Bank_1 IFR_0` region to utilize flash remap feature
- MCUBoot header size is set to 1024 bytes
- Signing algorithm is ECDSA-P256 (TinyCrypt)
- Write alignment is 16 bytes
- MCUBoot is configured to use its `DIRECT_XIP` image handling strategy
- Flash swapping HW feature is used for zero-overhead image swapping
- To manually enter serial recovery mode, keep the SW2 button pressed during reset, then release it

Image signing example:

    imgtool sign --key sign-ecdsa-p256-priv.pem
                 --align 16
                 --version 1.1
                 --slot-size 0x100000
                 --header-size 0x400
                 --pad-header
                 ota_mcuboot_basic.bin
                 ota_mcuboot_basic.SIGNED.bin

Note that for the first image flashed manually together with the bootloader
additional imgtool options `--pad` and `--confirm` must be used. Otherwise
the bootloader would reject the image for missing data in the trailer area.

If the toolchain used for build has problems with writing the bootloader
to the IFR flash region, an alternative can be to use NXP blhost utility
or *MCUXpresso Secure Provisioning Tool*.

**IMPORTANT:**
The CMPA region must be configured so that ROM loader boots from the IFR
region. This is done by setting the `BOOT_CFG.BOOT_SRC` field to `0b10`. This
value directs the ROM loader to start execution from the IFR region where
MCUboot is located. Default CMPA setup with this particular field set is
attached to this example in `cmpa.bin` binary file.
To flash this configuration together with MCUboot binary the following blhost
commands can be used.  Note that the board must be in the ISP mode to use
the following commands:

    #Erase IFR0 Bank 1 region
    blhost -u 0x1FC9,0x014F -- flash-erase-region 0x01008000 32768 0

    #Flash MCUboot
    blhost -u 0x1FC9,0x014F -- write-memory 0x01008000 mcuboot_opensource.bin

    #Flash CMPA
    blhost -u 0x1FC9,0x014F -- write-memory 0x01004000 cmpa.bin

### Custom configuration - Encrypted XIP

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x00000000 | 0x0003FFFF |  256kB |
| Primary slot   | 0x00040000 | 0x0011FFFF |  896kB |
| Secondary slot | 0x00120000 | 0x001FFFFF |  896kB |

This custom build generates a project with predefined configuration to utilize Encrypted XIP mode. For more information please see [Encrypted XIP and MCUboot](../../../../ota_examples/_doc/encrypted_xip.md).

Note: Default layout setup using the bootloader in IFR region limits usage of some features such as hardware accelerated mbedTLS (due size of IFR region) or encrypted XIP (due limitation of ROM). This custom configuration moves the bootloader to main flash array.

- MCUBoot header size is set to 1024 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 16 bytes
- MCUBoot is configured for `Encrypted XIP mode` using modified `MCUBOOT_OVERWRITE_ONLY` image update strategy
- MCUboot uses ECDH-P256 to secure AES key for image encryption
- Primary slot is encrypted by NPX module to utilize encrypted XIP

Image signing example:

    imgtool sign --key sign-ecdsa-p256-priv.pem
                 --align 16
                 --version 1.1
                 --slot-size 0xE0000
                 --header-size 0x400
                 --max-sectors 111
                 --pad-header
                 -E enc-ec256-pub.pem
                 ota_mcuboot_basic.bin
                 ota_mcuboot_basic.SIGNED.bin

Project building example:

The project is built using `west` tool. For more information please see [MCUXpresso SDK documentation](https://mcuxpresso.nxp.com/mcuxsdk/latest/html/introduction/README.html).

Bootloader:
~~~
west build -p always examples/ota_examples/mcuboot_opensource -b frdmmcxn947t --config debug --toolchain armgcc -Dcore_id=cm33_core0 -DCONF_FILE="examples/ota_examples/_custom_cfg/mcxn/mcuboot_opensource.conf"
~~~

OTA application:
~~~
west build -p always examples/ota_examples/ota_mcuboot_basic -b frdmmcxn947t --config debug --toolchain armgcc -Dcore_id=cm33_core0 -DCONF_FILE="examples/ota_examples/_custom_cfg/mcxn/ota_mcuboot_basic.conf"
~~~