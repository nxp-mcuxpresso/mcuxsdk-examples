# Encrypted XIP using IPED

This document extends the documentation of [MCUBoot and encrypted XIP in OTA examples](encrypted_xip.md) and provides a walkthrough to evaluate it on devices with IPED module. 

## 1. Introduction

IPED (Inline Prince Encryption/Decryption for off-chip flash) is encryption unit for external flash specific for NXP RW61x, RT700 and MCXN MCUs. 

Note: __The extension currently supports only IPED module based on GCM algorithm in RW61x devices.__

Following image shows configuration of metadata structure used for devices with IPED.

![Image](encrypted_xip_pics/iped_metadata.jpg)


The IPED engine generates an 8-byte authentication tag for each 32-byte block of encrypted data. When storing this encrypted data in Flash memory, the FlexSPI controller organizes it in a specific pattern:

* Each block of 32 bytes of ciphertext is followed by 8 bytes of authentication tag data (total: 40 bytes per unit)
* The authentication tags are stored physically in flash but are hidden from the CPU's logical address space (AHB read, fetch) - the CPU only sees the decrypted payload
* Due to this interleaving scheme, the actual physical flash consumption is (5/4)× the logical address space visible to the CPU

Following image shows an example of a valid IPED regions configuration - logical to physical address mapping

![Image](encrypted_xip_pics/iped_data_interleaving.jpg)

There are several points to be aware when utilizing IPED in an OTA process

* Resulting consumption of physical memory
	* range of IPED region is defined in terms of logical address but the physical memory consumption is 1.25× the logical memory consumption
	* OTA process must ensure that installed OTA image doesn't overlap the maximal size of IPED region, for example by adjusting the output binary size in linker file and/or doing checks of the image size before the encryption process
* Flash operations have to	satisfy boundaries of the flash page/sector size and encryption unit size
  * due internal software arbitration use only ROM IAP for flash writes
  * always write chunks of data aligned to 4 * page size, no partial writes are allowed - pad last data chunk with dummy bytes

The whole IPED initialization, metadata handling and image re-encryption are resolved in `encrypted_xip_platform_iped.c`, `bootutil_hooks.c` and flash backend porting layer.

Additional information for IPED in RW61x can be found in its reference manual.

## 2. Bootloader encryption

Note: Encrypting the mcuboot partition is required only for case when a private key used for offline encryption is embedded in bootloader code as C array (default setting in OTA examples).

To simplify the workflow, the MCUXpresso Secure Provisioning Tool (SEC tool) is used.

To provision the device and encrypt the bootloader perform the following steps:

1. Erase the device
2. Build `mcuboot_opensource`
3. Get the device into ISP mode 
    * Typically on development boards hold the ISP button and press the reset button
4. Open the SEC tool and create new workspace for RW61x target device
    * Test the ISP connection in SEC tool
5. Switch to PKI management tab
    * Click __Generate keys__ (leave default settings)
6. Build Image
    * Boot: __Encrypted (IPED) Plain__
    * Select or `mcuboot_opensource` output binary or ELF image as __Source executable image__
    * Lifecycle: __Develop, OTP__
    * Select an __authentication key__ and generate __CUST_MK_SK__ and __OEM SB seed__
    * Click __Build image__
7. Configure IPED regions
    * Click __IPED regions__
    * Configure Region 0 for MCUBoot partition as shown in following image (Region 1 is reserved for execution slot)
8. Write image
    * Click __Write image__

Note: This operation provisions the device with __RKTH__ and __CUST_MK_SK__ permanently, but the board will still be usable for development purposes as OTP BOOT_CFG0 (fuseword 15) remains intact. __An user is advised to save SEC tool workspace (or atleast the keys somewhere) for future use.__

![Image](encrypted_xip_pics/iped_bootloader_encryption.jpg)