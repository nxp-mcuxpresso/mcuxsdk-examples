# Encrypted XIP using IPED

This document extends the documentation of [MCUBoot and encrypted XIP in OTA examples](encrypted_xip.md) and provides an additional information related to the IPED module.

Note: __The extension currently supports only IPED module based on GCM algorithm in RW61x devices.__

## 1. Introduction

IPED (Inline Prince Encryption/Decryption for off-chip flash) is encryption unit for external flash specific for NXP RW61x, RT700 and MCXN MCUs. 

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
  * due internal software arbitration only ROM IAP for flash writes can be used
  * size of data chunk must be aligned to 4 * page size, no partial writes are allowed - last data chunk must be padded with dummy bytes

The whole IPED initialization, metadata handling and image re-encryption are resolved in `encrypted_xip_platform_iped.c`, `bootutil_hooks.c` and flash backend porting layer.

Additional information for IPED in RW61x can be found in its reference manual.

### 1.1 Metadata structure

The structure follows ROM code but the IPED configuration is handled manually from mcuboot context.

```
Metadata Flash Sector (IPED)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

 0                    ┌─────────────────────────────────────────────────────┐
                      │  tag           [4 bytes]                            │
                      │  params        [252 bytes]                          │
                      │    configId    [4 bytes]                            │
                      │    princeRnds  [4 bytes]                            │
                      │    regionOff   [4 bytes]                            │
                      │    ipedConfig  [60 bytes]  × 4 = 240 bytes          │
                      │      start     [4 bytes]                            │
                      │      end       [4 bytes]                            │
                      │      flags     [1 byte]                             │
                      │      padding   [3 bytes]                            │
                      │      encIv     [48 bytes]                           │
                      │        iv      [16 bytes]                           │
                      │        cipher  [16 bytes]                           │
                      │        tag     [16 bytes]                           │
 +256                 ├╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌┤
                      │                                                     │
                      │              (erased 0xFF)                          │
                      │                                                     │
 SECTOR_SIZE/2        ╠═════════════════════════════════════════════════════╣
                      │                                                     │
                      │              (erased 0xFF)                          │
                      │                                                     │
 SECTOR_SIZE-32       ├─────────────────────────────────────────────────────┤
                      │  enc_confirm_t [32 bytes]                           │
 SECTOR_SIZE          └─────────────────────────────────────────────────────┘
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

```
flexspi_nor_mem_image_iped_config_t (256 bytes, 16-byte aligned):
  [  0..  3]  tag                        (4 bytes)   "IPCB" (0x42435049) validity marker
  [  4..  7]    configId                 (4 bytes)   engine selector: kNBOOT_MemCryptIped (0x95959595)
  [  8.. 11]    ipedPrinceRounds         (4 bytes)   number of PRINCE rounds: 12 or 22
  [ 12.. 15]    regionOffset             (4 bytes)   lowest IPED context register index
  [ 16..255]    ipedConfig[4]          (240 bytes)   4 × nboot_iped_region_config_t (60 bytes each):
  [ +0.. +3]      startAddress           (4 bytes)   IPEDx_START register value
  [ +4.. +7]      endAddress             (4 bytes)   IPEDx_END register value
  [ +8.. +8]      regionFlags            (1 byte)    bit0=enabled, bit1=locked
  [ +9..+11]      padding                (3 bytes)   explicit alignment padding
  [+12..+59]      encryptedIv           (48 bytes)   AES-GCM protected IV blob:
  [+12..+27]        iv                  (16 bytes)   AES-GCM nonce used to encrypt the IV
  [+28..+43]        ciphertext          (16 bytes)   encrypted IPED region IV
  [+44..+59]        tag                 (16 bytes)   AES-GCM authentication tag

enc_confirm_t (32 bytes):
  [  0.. 31]  magic                  (32 bytes)   written last in a separate flash page — confirms config block
                                                  was fully persisted; absence signals incomplete OTA update
```

## 2. Encryption of MCUboot (OVERWRITE_ONLY only)

Encrypting the mcuboot partition is required only for case when a private key used for offline encryption is embedded in bootloader code as C array, otherwise, it's optional.

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

## 3. Initial SB3 image, OTA SB3 image (FLASH_REMAP only)

__Note: Encrypted XIP with FLASH_REMAP support is currently in an experimental state. The mode can be evaluated only with ota_mcuboot_basic example__

Due of customized placement of configuration structures out of FCB, an initial SB3 is required for safe deployment during end-product manufacturing. SB3 container can be also used as secure capsule for signed image providing an encryption during the OTA image transport. Additional information can be found ib [SB3 documentation](sb3_common_readme.md)

Note: The device must be provisioned with __RKTH__ and __CUST_MK_SK__. For provisioning, you can follow the previous chapter.

To simplify the workflow, the MCUXpresso Secure Provisioning Tool (SEC tool) is used.

1. Build `ota_mcuboot_basic` and sign image by `imgtool` as usual.
2. Look into `ota_examples/\_common/sb3_templates` and `ota_examples/\_common/binaries` directories and copy the template and additional binaries to your `$sec_tool_workspace`
    * `rw61x_IPED_initial_image.yaml` for the initial image
    * `rw61x_IPED_ota_slot0_image.yaml` or `rw61x_IPED_ota_slot1_image.yaml` for OTA image
    * `iped_conf_magic_page.bin` magic (confirmation) for configuration structure
    * `slot_trailer_page.bin` MCUboot slot trailer with confirmation flag (required for DIRECT-XIP mode)
3. Extract the FCB from `mcuboot_opensource` binary and place it in `source_images` folder
    * `nxpimage utils binary-image extract -b mcuboot_opensource.bin -a 0x400 -s 512 -o parsed_fcb.bin`
3. In SEC tool open __Tools/SB Editor__ and click __Import__ to import the template
    * Check and eventually fix paths to keys and image binary
    * click __Generate__
    * alternatively use SPSDK directly:  `nxpimage sb31 export -c template.yaml`

For initial image:

1. Get the device into ISP mode
    * Typically on development boards hold the ISP button and press the reset button
    * Test the ISP connection in SEC tool
2. The initial image can be loaded using `blhost`
    * `blhost -t 100000 -p COM3,115200 receive-sb-file initial_image_slot_0.sb`

For OTA update using SB3 follow instructions [MCUBoot and encrypted XIP in OTA examples](encrypted_xip.md).
