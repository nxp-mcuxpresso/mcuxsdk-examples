# Using the NXP MCUXpresso SDK MCUboot with a Zephyr Application

> **Draft / experimental - provided as-is, without warranty.**
> This document describes an experimental proof of concept. It is **not** a
> production-ready, fully supported flow. Several known issues in the Zephyr
> ecosystem and the Zephyr OTA / SMP API currently interfere with this use case -
> for example, they can break encrypted XIP, Wi-Fi initialization, and related
> functionality. All such issues are tracked and planned to be fixed in the NXP
> downstream, following the corresponding upstream changes. Until then, use this
> guide for evaluation only and expect rough edges.

## 1. Overview and Scope

The NXP MCUXpresso SDK fork of MCUboot provides features that are not (yet)
available in the MCUboot integrated into Zephyr's sysbuild. Typical reasons to
prefer the NXP fork include:

- **Hardware crypto accelerators** - offloading image signature/decryption to
  the on-chip security engines instead of a software crypto backend.
- **Encrypted XIP (execute-in-place)** - on-the-fly flash encryption/decryption
  using NXP modules such as BEE, OTFAD, IPED, or NPX, so the application image
  stays encrypted at rest in external or on-chip flash and is decrypted
  transparently during execution.
- Other NXP-specific bootloader behaviors and provisioning flows built around
  the MCUXpresso SDK ecosystem.

These features are currently not available in the MCUboot that Zephyr's sysbuild
builds. Until equivalent upstream support exists, a practical approach is to
**replace the Zephyr sysbuild MCUboot with the NXP MCUXpresso SDK MCUboot fork**
and let the Zephyr application be booted (and OTA-updated) by the NXP
bootloader. This has trade-offs (see the mode-specific notes below), but it lets
you evaluate and ship these NXP features today with a standard Zephyr
application.

This document is a **proof of concept** that demonstrates the approach using
**Encrypted XIP** with a Zephyr `smp_svr` application on the NXP FRDM-RW612
board. FRDM-RW612 uses the **IPED** encryption module, so IPED is the concrete
worked example here; the same alignment principles apply to the other supported
encryption modules (BEE, OTFAD, NPX) and boards - see
[Encrypted XIP and MCUboot](encrypted_xip.md) for the module/board matrix and
the per-module documents for module specifics.

The PoC proves the two ecosystems can be made compatible by aligning the flash
partition layout, the image signing/encryption keys, and the image header
format.

> **Important - the Zephyr sysbuild MCUboot is a dummy here.**
> When Zephyr is built with `SB_CONFIG_BOOTLOADER_MCUBOOT=y`, sysbuild produces
> its own MCUboot image. In this approach that MCUboot image is **never flashed**
> and is effectively a throwaway. Its only purpose is to make sysbuild lay out
> and (optionally) sign/encrypt the application image at the correct offset. The
> **real** bootloader that runs on the device is the one built from the NXP
> MCUXpresso SDK.

## 2. Prerequisites

- Two repositories checked out:
  - Zephyr west workspace
  - NXP MCUXpresso SDK west workspace
- Toolchains:
  - `armgcc` (or IAR) for the NXP SDK MCUboot
  - Zephyr SDK for the `smp_svr` application
- Host tools: `imgtool` (from MCUboot), a flashing tool (JLink / LinkServer),
  and `mcumgr` for OTA.

## 3. Flash Partition Layout

Both the NXP bootloader and the Zephyr application must agree on the exact same
layout (the Zephyr default is used here). The offsets and the encryption-metadata
regions are board- and module-specific; the values below are the IPED /
FRDM-RW612 worked example. All offsets are relative to the FlexSPI base
`0x08000000`.

| Region              | Offset     | Absolute address | Size          | Encryption region |
|---------------------|------------|------------------|---------------|-------------------|
| MCUboot bootloader  | `0x000000` | `0x08000000`     | 128 kB        | 0 (ROM)           |
| slot0 / image-0     | `0x020000` | `0x08020000`     | 3 MB          | 1 (custom)        |
| slot1 / image-1     | `0x320000` | `0x08320000`     | 3 MB          | x                 |
| ENC metadata slot0  | `0x620000` | `0x08620000`     | (flash sector)| x                 |
| ENC metadata slot1  | `0x621000` | `0x08621000`     | (flash sector)| x                 |

**Source of truth on the Zephyr side:**
`zephyr/boards/nxp/frdm_rw612/frdm_rw612_common.dtsi`
(`boot_partition`, `slot0_partition`, `slot1_partition`, `storage_partition`).

Note: `ENC metadata slot1` is a dummy here. The image in slot 1 (the candidate
      image) is encrypted offline by the MCUboot imgtool. Assigning an
      encryption region to slot 1 would conflict with the Zephyr OTA / SMP API.

For the encryption-metadata layout and the module-specific region rules (for
example, how many regions the ROM initializes and which one the bootloader uses
for the execution slot) see the relevant module document, e.g.
[Encrypted XIP using IPED](encrypted_xip_iped.md).


## 4. Part A - Build the NXP SDK enc-XIP Bootloader

> **Note:** For simplicity the NXP MCUboot fork keeps its default linker layout
> (256 kB / 2 MB / 2 MB), not the Zephyr layout (128 kB / 3 MB / 3 MB). The
> MCUboot build typically occupies less than 128 kB, so it does not overlap into
> the primary slot. Only the application slot addresses are overridden via the
> custom Kconfig below.

### 4.1 Custom configuration file


Create a custom config file `mcuboot_opensource_zephyr_poc.conf` in the NXP SDK
west workspace. The following is the IPED / FRDM-RW612 example; adjust the
addresses to your board's partition layout (Section 3) and select the upgrade
mode / encryption module for your target. Full contents:

```
# Custom configuration for encrypted XIP (IPED) evaluation for a Zephyr app
# boards: frdmrw612
# layout 128kB mcuboot, 3MB/3MB slots, 4kB encryption configuration metadata for slot 0

CONFIG_MCUX_COMPONENT_middleware.mcuboot.serial_recovery=n
CONFIG_MCUX_COMPONENT_component.psa_crypto_driver.els_pkc=n
CONFIG_BOOT_CUSTOM_DEVICE_SETUP=y
CONFIG_BOOT_MODE_ENCRYPTED_XIP_OVERWRITE=y
CONFIG_BOOT_FLASH_ACT_APP_ADDRESS=0x08020000
CONFIG_BOOT_FLASH_CAND_APP_ADDRESS=0x08320000
CONFIG_BOOT_FLASH_SLOT0_ENC_CFG_ADDRESS=0x08620000
CONFIG_BOOT_FLASH_SLOT1_ENC_CFG_ADDRESS=0x08621000
```

These addresses match the Zephyr DTS layout from Section 3. When
`CONFIG_BOOT_CUSTOM_DEVICE_SETUP=y`, `flash_partitioning.h` reads the addresses
from Kconfig rather than using the SDK defaults. For more ways to enable
encrypted XIP (including the GUI config flow) see
[Kconfig and customization of OTA examples](kconfig_customization.md).


### 4.2 Build command

```
west build -p always mcuxsdk/examples/ota_examples/mcuboot_opensource --toolchain armgcc --config flash_debug -b frdmrw612 -DCONF_FILE="mcuboot_opensource_zephyr_poc.conf" -d nxpmcuboot_zephyr_poc
```

## 5. Part B - Build the Zephyr smp_svr Application

### 5.1 Customize sysbuild.conf to switch upgrade mode to OVERWRITE_ONLY

`sysbuild.conf`:

```
# Enable MCUboot bootloader support (dummy image, not flashed)
SB_CONFIG_BOOTLOADER_MCUBOOT=y
SB_CONFIG_MCUBOOT_MODE_OVERWRITE_ONLY=y
SB_CONFIG_BOOT_ENCRYPTION=y
```

Note: OVERWRITE_ONLY has no revert/confirm capability - once the candidate
image is written to the primary slot there is no automatic rollback to the
previous image if the new one fails to boot.

### 5.2 App transport and versioning


- Use `shell.conf` (mcumgr over the shell transport) because console, shell, and
  mcumgr share the same `flexcomm3` UART. A raw mcumgr UART transport conflicts
  with the shell on that shared port.
- Use the `VERSION` file to control the image version (optional).

### 5.3 Build

Build the app with west/sysbuild as usual for `frdm_rw612`.

```
west build -b frdm_rw612 -d rw612_smp_svr --sysbuild applications/smp_svr -- -DEXTRA_CONF_FILE="shell.conf"
```

The unsigned application binary is produced in the app image build directory
(`rw612_smp_svr/smp_svr/zephyr/zephyr.bin`). Ignore `zephyr.signed.bin`
and `zephyr.signed.encrypted.bin` as these are created with Zephyr signature and
encryption keys (see Part C).

Ignore `rw612_smp_svr/mcuboot/zephyr/` as this is the Zephyr MCUboot build.

Note: because the app is an MCUboot image, the Zephyr linker already reserves
0x400 bytes of header space at the start of the binary. **This reserved 0x400
matters for signing** - see Part C.


## 6. Part C - Signing the App with the NXP Keys

Sign the Zephyr binary with the NXP SDK's signing key, and encrypt with the NXP
SDK's encryption public key, **WITHOUT `--pad-header`** (the Zephyr binary
already reserves the 0x400 header space, so `--pad-header` would add a second
header - see the note at the end of Part B, Section 5.3):

```
imgtool sign \
  --key   <path>/mcuxsdk/middleware/mcuboot_opensource/boot/nxp_mcux_sdk/keys/sign-ecdsa-p256-priv.pem \
  --header-size 0x400 \
  --align 4 \
  --slot-size 0x300000 \
  --max-sectors 768 \
  --version "1.0.0" \
  -E <path>/mcuxsdk/middleware/mcuboot_opensource/boot/nxp_mcux_sdk/keys/enc-ec256-pub.pem \
  <zephyr-input>.bin \
  app-signed-encrypted.bin
```

Note: the MCUboot signing and encryption key files used here are for test and
debug purposes only. For production, generate your own key set and update the
contents of `middleware\mcuboot_opensource\boot\nxp_mcux_sdk\keys\` accordingly.

## 7. Part D - Flash initial binaries

There are two options.

**Option 1 - flash manually (test / debug, MCUboot not encrypted):**

- Flash the customized NXP bootloader
  `nxpmcuboot_zephyr_poc\mcuboot_opensource.bin` to `0x08000400`
  (note the 0x400 offset here).
- Flash `app-signed-encrypted.bin` to `0x08320000`.

**Option 2 - create an initial SB3 file using the SEC tool:**

- Place MCUboot as the bootable image within encryption region 0, and add
  `app-signed-encrypted.bin` as an additional image targeting `0x08320000`.

Note: Option 2 is the recommended approach for mass production, especially when
an OEM manufactures your product, because the SB3 binary is bound to a private
master key that you provision and embed in each device instance. See
[RW61x - OTA update by using SB3 file](sb3_rw61x_readme.md) for the SB3 flow.

### 7.1 Run the code

MCUboot inspects both slots. Slot 0 is empty, but it finds a signed and
encrypted image in slot 1, so it re-encrypts that image (from the offline
AES-128 encryption into the hardware encrypted-XIP encryption) into slot 0.


Expected output (IPED / FRDM-RW612 worked example):

```
hello sbl.
Bootloader Version 2.4.0
Built Sep 12 2026 10:41:25
Toolchain GCC 14.2.1 20241119
Upgrade mode: ENCRYPTED XIP: OVERWRITE ONLY
context_boot_go
Image index: 0, Swap type: none
boot_validate_slot: slot 0, expected_swap_type 0
boot_validate_slot: slot 1, expected_swap_type 0
boot_enc_load: slot 1
...
Image 0 upgrade secondary slot -> primary slot
Erasing the primary slot
Encrypted XIP: Erasing configuration block slot 0
...
Image 0 copying the secondary slot to the primary slot: 0x161fd bytes
boot_write_magic: fa_id=0 off=0x2ffff0 (0x31fff0)
erasing secondary header
erasing secondary trailer
...
Bootloader chainload address offset: 0x20000
Reset_Handler address offset: 0x20400
Jumping to the image



uart:~$
```

## 8. Part E - OTA Update Flow

1. Build a new app with a bumped `VERSION`.
2. Sign it per Part C (no `--pad-header`).
3. Upload over the shell mcumgr transport (UART) or [AuTerm](https://github.com/thedjnK/AuTerm/), e.g.:
   - `mcumgr ... image upload app-signed.bin`
   - `mcumgr ... image list`
   - `mcumgr ... image test <hash-of-new-image>`
4. Reset the board; MCUboot overwrites the primary slot with the (encrypted)
   candidate.

Known issue: `mcumgr ... image list` cannot read the active image in slot 0 when
the execution slot is encrypted, because the code reads flash content through
direct flash IP access that bypasses the on-the-fly decryption module, so it
only sees ciphertext. Slot 1 is not encrypted, so the SMP server can still fetch
its version and compute the hash that the user needs to perform the OTA update.


```
$ mcumgr -c rw612_serial image list
Images:
Split status: N/A (0)
$ mcumgr -c rw612_serial image upload app-signed-encrypted.bin
 88.50 KiB / 88.50 KiB [=======================================================================] 100.00% 3.39 KiB/s 26s
Done
$ mcumgr -c rw612_serial image list
Images:
 image=0 slot=1
    version: 1.0.0
    bootable: true
    flags:
    hash: e3aab26656e757b569b58864271638118e2409c84deac1cea82d5badb6ea4ffd
Split status: N/A (0)
$ mcumgr -c rw612_serial image test e3aab26656e757b569b58864271638118e2409c84deac1cea82d5badb6ea4ffd
Images:
 image=0 slot=1
    version: 1.0.0
    bootable: true
    flags: pending
    hash: e3aab26656e757b569b58864271638118e2409c84deac1cea82d5badb6ea4ffd
Split status: N/A (0)
```

## 9. Encryption Constraints and Module Specifics

Each encryption module (BEE, OTFAD, IPED, NPX) has its own metadata layout,
region rules, key-protection scheme, and constraints (for example, physical
flash overhead, region collisions, and provisioning requirements). These are
**not** repeated here - consult the module document for your board:

- [Encrypted XIP and MCUboot](encrypted_xip.md) - overview, upgrade modes, and
  the module/board matrix.
- [Encrypted XIP using IPED](encrypted_xip_iped.md) - IPED (RW61x). Includes the
  practical good-practice notes, pitfalls, and troubleshooting relevant to this
  PoC (flash overhead, region usage, debugger observations, PRINCE reset loops).
- [Encrypted XIP using BEE](encrypted_xip_bee.md) - BEE (RT10xx).
- [Encrypted XIP using OTFAD](encrypted_xip_otfad.md) - OTFAD (RT116x/RT117x).
- [Encrypted XIP and NPX](encrypted_xip_npx.md) - NPX (MCXN).

## 10. Appendix - Key Paths
- Zephyr partition layout:
  `zephyr/boards/nxp/frdm_rw612/frdm_rw612_common.dtsi`
- NXP encrypted XIP documentation:
  `mcuxsdk/examples/ota_examples/_doc`
- NXP SDK MCUboot example:
  `mcuxsdk/examples/ota_examples/mcuboot_opensource`
- NXP SDK keys:
  `mcuxsdk/middleware/mcuboot_opensource/boot/nxp_mcux_sdk/keys/`
  - `sign-ecdsa-p256-priv.pem` (image signing)
  - `enc-ec256-pub.pem` (image encryption)
- Zephyr app: `applications/smp_svr`
  - `sysbuild.conf`, `shell.conf`, `VERSION`
