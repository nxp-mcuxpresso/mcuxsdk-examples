# Encrypted XIP using BEE (Bus Encryption Engine)

This document extends the documentation of [MCUBoot and encrypted XIP in OTA examples](encrypted_xip.md) and provides an additional information related to the BEE module. 

## 1. Introduction

BEE is specific for RT10xx (except RT1010). The engine supports up to two separate regions using two separate AES keys. In the ota examples, BEE region 1 is used for encrypting the execution slot and BEE region 0 is reserved for a bootloader.

BEE configuration blocks are organized as __EPRDB__ (Encrypted Protection Region Descriptor Block), where the __EPRDB__ is encrypted using AES-CBC mode with AES key and IV located in __KIB__ (Key Info Block). The __KIB__ is encrypted as __EKIB__ (Encrypted KIB) using a key provisioned by the user. Each BEE region has its __PRDB/KIB pair__.

The EKIB is decrypted by a key based on selection in `BEE_KEYn_SEL` fuse:

* __Software key__
	* default value in `BEE_KEYn_SEL`
	* evaluating BEE without fusing the device
* __SW-GP2__
	* fused by user and typically used for offline encryption
	* limited funcionality due hardware bugs, see errata
	* not supported in the examples
* __OTPMK__
	* provisioned by NXP in factory
	* unique per device instance - prevents image cloning
	* __recommended__

Firmware in execution slot is de/encrypted using AES-CTR combining nonce extracted from PRDB and this device key. The extension automatically detects device key by evaluating `BEE_KEYn_SEL` fuse.

The whole BEE initialization and encryption metadata handling is resolved in module `encrypted_xip_platform_bee.c`.

Additional information can be found in Security Reference Manual of target device and in application notes AN12800, AN12852 and AN12901.

### 1.1 Metadata structure

The structure follows ROM code but the BEE configuration is handled manually from mcuboot context.

```
Metadata Flash Sector (BEE)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

 0                    ┌─────────────────────────────────────────────────────┐
                      │  ekib          [32 bytes]  encrypted kib_t          │
                      │    aes_key     [16 bytes]                           │
                      │    iv          [16 bytes]                           │
 +32                  ├─────────────────────────────────────────────────────┤
                      │  eprdb         [256 bytes] encrypted prdb_t         │
                      │    tagl/tagh   [8 bytes]                            │
                      │    version     [4 bytes]                            │
                      │    fac_count   [4 bytes]                            │
                      │    enc_region  [64 bytes]                           │
                      │      start     [4 bytes]                            │
                      │      end       [4 bytes]                            │
                      │      aes_mode  [4 bytes]                            │
                      │      nonce     [16 bytes]                           │
                      │      reserved  [36 bytes]                           │
                      │    fac_region  [32 bytes]  × 4 = 128 bytes          │
                      │    reserved    [48 bytes]                           │
 +288                 ├╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌┤
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
bee_cfg_ctx_t (288 bytes, 16-byte aligned):
  [  0.. 31]  ekib                    (32 bytes)  AES-ECB encrypted KIB
  [  0.. 15]    aes_key               (16 bytes)  AES-128 key for BEE runtime decryption of execution slot
  [ 16.. 31]    iv                    (16 bytes)  AES Initial Vector for CBC encryption of PRDB
  [ 32..287]  eprdb                  (256 bytes)  AES-CBC encrypted PRDB (using aes_key + iv from KIB)
  [ 32.. 39]    tagl / tagh           (8 bytes)   fixed magic "TAG_EHDR"
  [ 40.. 43]    version               (4 bytes)   PRDB format version (0x56010000 = v1.0.0)
  [ 44.. 47]    fac_count             (4 bytes)   number of active FAC regions (1–3)
  [ 48..111]    enc_region           (64 bytes)   BEE-encrypted XIP region descriptor
  [ 48.. 51]      start              (4 bytes)    region start address, 4 kB aligned
  [ 52.. 55]      end                (4 bytes)    region end address, 4 kB aligned
  [ 56.. 59]      aes_mode           (4 bytes)    AES mode: 0 = ECB, 1 = CTR
  [ 60.. 63]      lock_option        (4 bytes)    BEE lock option
  [ 64.. 79]      nonce             (16 bytes)    upper 96 bits of AES-CTR counter (address field supplied by HW)
  [ 80..111]      reserved          (32 bytes)    padding / future use
  [112..143]    fac_region_0         (32 bytes)   FAC region: start / end / access mode (debug/read permissions)
  [144..175]    fac_region_1         (32 bytes)   FAC region (unused, zeroed)
  [176..207]    fac_region_2         (32 bytes)   FAC region (unused, zeroed)
  [208..239]    fac_region_3         (32 bytes)   FAC region (unused, zeroed)
  [240..287]    reserved            (48 bytes)    padding to reach exactly 256 bytes

enc_confirm_t (32 bytes):
  [  0.. 31]  magic                  (32 bytes)   written last in a separate flash page — confirms config block
                                                  was fully persisted; absence signals incomplete OTA update
```