# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# The LLC test region on MIMXRT2660-EVK is the on-board PSRAM behind XSPI1.
#
# Bring it up from the Boot ROM: USE_PSRAM makes mimxrt2660evk_boot_config.c
# link the board's real XMCD payload (selected by USE_PSRAM_W958D6) into the
# boot header, so the ROM configures XSPI1 and the device before the image
# runs. Every target of this example is XIP from XSPI0 NOR and therefore always
# carries that boot header, and the XMCD covers both board PSRAM variants, so
# this is the only bring-up the example needs - there is no software sequence
# here, and none that would work on both variants. The RT2xxx.cmake project
# settings define USE_PSRAM for the psram* targets only; this example needs it
# on every target, because the LLC test region is PSRAM on all of them.
mcux_add_configuration(
    CC "-DUSE_PSRAM=1"
    CX "-DUSE_PSRAM=1"
)
