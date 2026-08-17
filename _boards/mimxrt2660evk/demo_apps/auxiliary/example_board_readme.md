Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
No special settings are required.

Overview
========
The auxiliary demo is not a normal application and does not print anything to
the terminal. It is a lightweight helper image whose purpose depends on the
build target. See the target descriptions below for details.

Supported targets
=================

psram_only_debug / psram_only_release
--------------------------------------
Scenario: eMMC + PSRAM.

Although the build target is named psram_only, it uses the ram_only linker
file to produce an eMMC-bootable image.

Program this image into the eMMC to enable debugging of other projects'
psram_only target in the eMMC + PSRAM scenario.

xspi_nor_psram_debug / xspi_nor_psram_release
-----------------------------------------------
Scenario: XSPI NOR flash + PSRAM.

Program this image into the XSPI NOR flash to enable debugging of other
projects' psram_only target in the XSPI NOR flash + PSRAM scenario.

ram_only_debug / ram_only_release
-----------------------------------
Scenario: eMMC, no XSPI NOR flash.

This target avoids permanently burning a boot-mode fuse while still enabling
eMMC-based debugging. It sets the eMMC boot mode via a volatile shadow fuse
and performs a software reset so that the ROM boots from eMMC.

Because shadow fuses are not persistent across power cycles and there is no
XSPI NOR flash to re-apply the setting on each power-on, eMMC POR boot is not
supported on this build variant.

xspi_nor_debug / xspi_nor_release
-----------------------------------
Scenario: eMMC, with XSPI NOR flash.

This target avoids permanently burning a boot-mode fuse while still enabling
eMMC-based debugging. It sets the eMMC boot mode via a volatile shadow fuse
and performs a software reset so that the ROM boots from eMMC.

With XSPI NOR flash present, this image can be stored in NOR flash so that
the shadow fuse is re-applied on every power-on, enabling eMMC POR boot
without burning any OTP fuse.
